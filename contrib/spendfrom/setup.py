from distutils.core import setup
setup(name='PLCspendfrom',
      version='1.0',
      description='Command-line utility for polcoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@polcoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
