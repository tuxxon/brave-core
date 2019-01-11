#!/usr/bin/env python
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

from mock import MagicMock

class MockImport(object):
    def __import__(self):
        pass


class Repo():
    def __init__(self):
        self.releases = self.Releases()

    class Releases():
        def __init__(self):
            self._releases = []

        def get(self):
            return self._releases

        def __call__(self, id):
            obj = [x for x in self._releases if x.id == id]
            if len(obj) == 1:
                print('call(' + str(id) + ') returned tag_name: "' + obj[0].tag_name + '"')
                return obj[0]
            return None

class Asset():
    def __init__(self, id=-1, name=""):
        self.id = id
        self.name = name
        self.delete = MagicMock()

class Assets():
    def __init__(self):
        self._assets = []
        self.post = MagicMock()

    def get(self):
        return self._assets

class Release():
    def __init__(self):
        self.id = -1
        self.tag_name = ""
        self.draft = True
        self.assets = Assets()

    def __getitem__(self, name):
        print('getitem for ' + name)

        if name == 'id':
          return self.id
        elif name == 'tag_name':
          return self.tag_name
        elif name == 'draft':
          return self.draft
        elif name == 'assets':
          return self.assets

    def __getattr__(self, attr):
        print('Not found: ' + attr)
        return None
