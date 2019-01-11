#!/usr/bin/env python

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import unittest
import os
import upload
from githubmock import Repo, Release, Asset
from mock import MagicMock

dirname = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dirname, '..'))

# get an existing release (in draft status) from GitHub given a tag name
class TestGetDraft(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()

    def test_returns_existing_draft(self):
        self.repo.releases._releases = [{'tag_name': 'test', 'draft': True}]
        self.assertEquals(upload.get_draft(self.repo,
                                           'test')['tag_name'], 'test')

    def test_fails_on_existing_release(self):
        self.repo.releases._releases = [{'tag_name': 'test', 'draft': False}]
        self.assertRaises(UserWarning, upload.get_draft, self.repo, 'test')

    def test_returns_none_on_new_draft(self):
        self.repo.releases._releases = [{'tag_name': 'old', 'draft': False}]
        upload.get_draft(self.repo, 'new')
        self.assertEquals(upload.get_draft(self.repo, 'test'), None)


class TestGetBravePackages(unittest.TestCase):

    get_pkgs_dir = os.path.join(os.path.dirname(
        os.path.realpath(__file__)), 'test_get_pkgs')
    _is_setup = False

    def setUp(self):
        if not self._is_setup:
            for chan in ['Release', 'Dev', 'Beta']:
                if chan not in 'Release':
                    for mode in ['Stub', 'Standalone']:
                        name = 'BraveBrowser{}{}Setup_70_0_56_8.exe'.format(
                            mode if mode not in 'Stub' else '', chan)
                        name32 = ('BraveBrowser{}{}Setup32_70_0_56_8.exe'
                                  .format(mode if mode not in 'Stub'
                                          else '', chan))
                        with open(os.path.join(self.get_pkgs_dir,
                                               'win32', name), 'w') as f:
                                f.write(name)
                        with open(os.path.join(self.get_pkgs_dir,
                                               'win32', name32), 'w') as f:
                                f.write(name32)
                else:
                    for mode in ['Stub', 'Standalone']:
                        name = 'BraveBrowser{}Setup_70_0_56_8.exe'.format(
                            mode if mode not in 'Stub' else '')
                        name32 = 'BraveBrowser{}Setup32_70_0_56_8.exe'.format(
                            mode if mode not in 'Stub' else '')
                        with open(os.path.join(
                                self.get_pkgs_dir, 'win32', name), 'w') as f:
                                f.write(name)
                        with open(os.path.join(
                                self.get_pkgs_dir, 'win32', name32), 'w') as f:
                                f.write(name32)
        self.__class__._is_setup = True

    def test_only_returns_dev_darwin_package(self):
        upload.PLATFORM = 'darwin'
        pkgs = list(upload.get_brave_packages(os.path.join(
            self.get_pkgs_dir, upload.PLATFORM), 'dev', '0.50.8'))
        self.assertEquals(pkgs, ['Brave-Browser-Dev.dmg'])

    def test_only_returns_beta_darwin_package(self):
        upload.PLATFORM = 'darwin'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'beta', '0.50.8'))
        self.assertEquals(pkgs, ['Brave-Browser-Beta.dmg'])

    def test_only_returns_release_darwin_package(self):
        upload.PLATFORM = 'darwin'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'release', '0.50.8'))
        self.assertEquals(pkgs, ['Brave-Browser.dmg',
                                 'Brave-Browser.pkg'])

    def test_only_returns_dev_linux_packages(self):
        upload.PLATFORM = 'linux'
        pkgs = list(upload.get_brave_packages(os.path.join(self.get_pkgs_dir,
                                                           upload.PLATFORM),
                                              'dev', '0.50.8'))
        self.assertEquals(sorted(pkgs),
                          sorted(['brave-browser-dev-0.50.8-1.x86_64.rpm',
                                  'brave-browser-dev_0.50.8_amd64.deb']))

    def test_only_returns_release_linux_packages(self):
        upload.PLATFORM = 'linux'
        pkgs = list(upload.get_brave_packages(os.path.join(self.get_pkgs_dir,
                                                           upload.PLATFORM),
                                              'release', '0.50.8'))
        self.assertEquals(sorted(pkgs),
                          sorted(['brave-browser-0.50.8-1.x86_64.rpm',
                                  'brave-browser_0.50.8_amd64.deb']))

    def test_only_returns_dev_win_x64_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'x64'
        pkgs = list(upload.get_brave_packages(os.path.join(
            self.get_pkgs_dir, upload.PLATFORM), 'dev', '0.56.8'))
        self.assertEquals(pkgs, ['BraveBrowserStandaloneDevSetup.exe',
                                 'BraveBrowserDevSetup.exe'])

    def test_only_returns_dev_win_ia32_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'ia32'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM), 'dev', '0.56.8'))
        self.assertEquals(pkgs, ['BraveBrowserStandaloneDevSetup32.exe',
                                 'BraveBrowserDevSetup32.exe'])

    def test_only_returns_beta_win_x64_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'x64'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'beta', '0.56.8'))
        self.assertEquals(pkgs, ['BraveBrowserStandaloneBetaSetup.exe',
                                 'BraveBrowserBetaSetup.exe'])

    def test_only_returns_beta_win_ia32_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'ia32'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'beta', '0.56.8'))
        self.assertEquals(pkgs, ['BraveBrowserStandaloneBetaSetup32.exe',
                                 'BraveBrowserBetaSetup32.exe'])

    def test_only_returns_release_win_x64_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'x64'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'release', '0.56.8'))
        self.assertEquals(sorted(pkgs), sorted(['BraveBrowserSetup.exe',
                                 'BraveBrowserStandaloneSetup.exe']))

    def test_only_returns_release_win_ia32_package(self):
        upload.PLATFORM = 'win32'
        os.environ['TARGET_ARCH'] = 'ia32'
        pkgs = list(upload.get_brave_packages(
            os.path.join(self.get_pkgs_dir, upload.PLATFORM),
            'release', '0.56.8'))
        self.assertEquals(sorted(pkgs), sorted(['BraveBrowserStandaloneSetup32.exe',
                                 'BraveBrowserSetup32.exe']))

# uploading a single file to GitHub
class TestUploadBrave(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()
        self.file_path = os.path.join(os.path.dirname(
            os.path.realpath(__file__)),
            'test_get_pkgs',
            'win32',
            'BraveBrowserSetup.exe')
        self.release = Release()
        self.release.id = 1
        self.release.tag_name = 'release-tag-here'
        self.asset = Asset(1, 'BraveBrowserSetup.exe')
        self.release.assets._assets.append(self.asset)
        self.repo.releases._releases = [self.release]
        self.repo.releases.assets = self.release.assets

        self._old_upload_sha256_checksum = upload.upload_sha256_checksum
        upload.upload_sha256_checksum = MagicMock()

        self._old_upload_io_to_github = upload.upload_io_to_github
        upload.upload_io_to_github = MagicMock()

    def tearDown(self):
        upload.upload_sha256_checksum = self._old_upload_sha256_checksum
        upload.upload_io_to_github = self._old_upload_io_to_github

    def test_calls_delete_if_already_exists(self):
        upload.upload_brave(self.repo, self.release, self.file_path)
        self.asset.delete.assert_called()

    def test_calls_upload(self):
        upload.upload_brave(self.repo, self.release, self.file_path)
        upload.upload_io_to_github.assert_called()

        args, kwargs = upload.upload_io_to_github.call_args
        self.assertTrue(args[0] == self.repo)
        self.assertTrue(args[2] == self.release.assets._assets[0].name)
        self.assertTrue(args[4] == 'application/zip')

    def test_retries_upload(self):
        print('lol')

    def test_calls_uploads_checksum(self):
        upload.upload_brave(self.repo, self.release, self.file_path)
        upload.upload_sha256_checksum.assert_called_with(self.release.tag_name, self.file_path)

if __name__ == '__main__':
    print unittest.main()

