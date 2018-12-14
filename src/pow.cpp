// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2016 The Polcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % Params().Interval() != 0)
    {
        if (Params().AllowMinDifficultyBlocks())
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + Params().TargetSpacing()*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % Params().Interval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < Params().Interval()-1; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < Params().TargetTimespan()/4)
        nActualTimespan = Params().TargetTimespan()/4;
    if (nActualTimespan > Params().TargetTimespan()*4)
        nActualTimespan = Params().TargetTimespan()*4;

    // Retarget
    uint256 bnNew;
    uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= Params().TargetTimespan();

    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();

    /// debug print
    LogPrintf("GetNextWorkRequired RETARGET\n");
    LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

unsigned int DarkGravityWave3(const CBlockIndex* pindexLast, const CBlockHeader *pblock) {
    //unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();
    /* current difficulty formula, darkcoin - DarkGravity v3, written by Evan Duffield - evan@darkcoin.io */
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    const CBlockHeader *BlockCreating = pblock;
    BlockCreating = BlockCreating;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 24;
    int64_t PastBlocksMax = 24;
    int64_t CountBlocks = 0;
    uint256 PastDifficultyAverage;
    uint256 PastDifficultyAveragePrev;
    uint256 bnProofOfWorkLimit = Params().ProofOfWorkLimit();
    int64_t nTargetSpacing = Params().TargetSpacing();

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) {
        // This is the first block or the height is < PastBlocksMin
        // Return minimal required work. (1e0fffff)
        return bnProofOfWorkLimit.GetCompact();
    }

    // loop over the past n blocks, where n == PastBlocksMax
    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        CountBlocks++;

        // Calculate average difficulty based on the blocks we iterate over in this for loop
        if(CountBlocks <= PastBlocksMin) {
            if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
            else { PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks)+(uint256().SetCompact(BlockReading->nBits))) / (CountBlocks+1); }
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        // If this is the second iteration (LastBlockTime was set)
        if(LastBlockTime > 0){
            // Calculate time difference between previous block and current block
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
            // Increment the actual timespan
            nActualTimespan += Diff;
        }
        // Set LasBlockTime to the block time for the block in current iteration
        LastBlockTime = BlockReading->GetBlockTime();

        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }
    
    // bnNew is the difficulty
    uint256 bnNew(PastDifficultyAverage);

    // nTargetTimespan is the time that the CountBlocks should have taken to be generated.
    int64_t nTargetTimespan = CountBlocks*nTargetSpacing;

    // Limit the re-adjustment to 3x or 0.33x
    // We don't want to increase/decrease diff too much.
    if (nActualTimespan < nTargetTimespan/3)
        nActualTimespan = nTargetTimespan/3;
    if (nActualTimespan > nTargetTimespan*3)
        nActualTimespan = nTargetTimespan*3;

    // Calculate the new difficulty based on actual and target timespan.
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    // If calculated difficulty is lower than the minimal diff, set the new difficulty to be the minimal diff.
    if (bnNew > bnProofOfWorkLimit){
        bnNew = bnProofOfWorkLimit;
    }
    
    // Some logging.
    // TODO: only display these log messages for a certain debug option.
    LogPrintf("Difficulty Retarget - Dark Gravity Wave 3\n");
    LogPrintf("Before: %08x %s\n", BlockLastSolved->nBits, uint256().SetCompact(BlockLastSolved->nBits).ToString().c_str());
    LogPrintf("After: %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());

    // Return the new diff.
    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    if (Params().AllowMinDifficultyBlocks())//ftestnet
    {
	if (pindexLast->nHeight+1 <= 2) { return GetNextWorkRequired_V1(pindexLast, pblock); }
	return DarkGravityWave3(pindexLast, pblock);
    }
    else
    {
	if (pindexLast->nHeight >= 1092600) { return DarkGravityWave3(pindexLast, pblock); }
        return GetNextWorkRequired_V1(pindexLast, pblock);
    }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, int nHeight)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (hash.ToString() == "5b0d84e63fa3dae7bb2ad21386ec5f6d146b70414bd825857636f91f4257d135" && nHeight == 0)
	{
	LogPrintf("Gensis pass\n");
	return true;
	}
    if (Params().SkipProofOfWorkCheck())
       return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
{
	LogPrintf("hash: %s\n", hash.ToString());
	LogPrintf("bnTarget: %s\n", bnTarget.ToString());
        return error("CheckProofOfWork() : hash doesn't match nBits");
}
/*else
{
	LogPrintf("hash: %s\n", hash.ToString());
	LogPrintf("bnTarget: %s\n", bnTarget.ToString());
}*/
    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}
