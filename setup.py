import os
import shutil
import subprocess
import setuptools
from setuptools.command.install import install
from setuptools.command.develop import develop
from setuptools.command.test import test


with open('./README.md') as fhandle:
    readme = fhandle.read()

_root_dir = os.path.dirname(os.path.realpath(__file__))


def _build_backend(root_dir):
    print('Compiling gramtools backend')
    cmake_dir = os.path.join(root_dir, 'cmake-build-debug')
    try:
        shutil.rmtree(cmake_dir, ignore_errors=False)
    except FileNotFoundError:
        pass
    subprocess.call(['mkdir', cmake_dir])
    subprocess.call('CC=gcc CXX=g++ cmake ..', cwd=cmake_dir, shell=True)

    return_code = subprocess.call(['make'], cwd=cmake_dir)
    if return_code != 0:
        print('ERROR: gramtools backend compilation returned: ', return_code)
        exit(-1)


def _test_backend(root_dir):
    cmake_dir = os.path.join(root_dir, 'cmake-build-debug')
    test_runner = os.path.join(cmake_dir, 'libgramtools', 'tests', 'test_main')
    test_dir = os.path.join(root_dir, 'libgramtools', 'tests')

    return_code = subprocess.call([test_runner], cwd=test_dir)
    if return_code != 0:
        print('ERROR: gramtools backend test runner returned: ', return_code)
        exit(-1)


class _InstallCommand(install):
    """pip3 install -vvv ./gramtools"""
    def run(self):
        _build_backend(_root_dir)
        _test_backend(_root_dir)
        install.run(self)


class _DevelopCommand(develop):
    """pip3 install -vvv --editable ./gramtools"""
    def run(self):
        _build_backend(_root_dir)
        _test_backend(_root_dir)
        develop.run(self)


class _TestCommand(test):
    """python3 setup.py test"""
    def run(self):
        _build_backend(_root_dir)
        _test_backend(_root_dir)
        test.run(self)


_package_data = {
    'gramtools': [
        'bin/gram',
        'lib/*',
        'utils/vcf_to_linear_prg.pl'],
}


setuptools.setup(
    name='gramtools',
    version='2.0',
    description='Genome inference and variant calling with a reference graph of genetic variation.',
    url='https://github.com/iqbal-lab-org/gramtools',
    long_description=readme,
    entry_points={
        'console_scripts': ['gramtools = gramtools.gramtools:run']
    },
    packages=setuptools.find_packages("."),
    package_data=_package_data,
    include_package_data=True,
    install_requires=[
        'scipy >= 1.0.1',
        'pyvcf >= 0.6.8',
        'py-cortex-api >= 1.0',
        'cluster_vcf_records >= 0.6.0',
    ],
    dependency_links=[
        'https://github.com/iqbal-lab-org/py-cortex-api/tarball/master#egg=py-cortex-api-1.0',
        'https://github.com/iqbal-lab-org/cluster_vcf_records/tarball/master#egg=cluster-vcf-records-0.6.0'
    ],
    test_suite='gramtools.tests',
    cmdclass={
        'install': _InstallCommand,
        'develop': _DevelopCommand,
        'test': _TestCommand,
    })
