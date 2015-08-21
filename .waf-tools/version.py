# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

from waflib.Configure import conf
from waflib import Utils
import os

def splitVersion(version):
    base = version.split('-')[0]
    split = [v for v in base.split('.')]
    return base, version, split

@conf
def getVersion(conf, submodule, **kw):
    tagPrefix = kw.get('tag', '%s-' % submodule)
    baseVersion = kw.get('base_version', '0.0.0')
    submodule = conf.path.find_node(submodule)

    gitVersion = baseVersion

    didGetVersion = False
    try:
        cmd = ['git', 'describe', '--always', '--match', '%s*' % tagPrefix]
        p = Utils.subprocess.Popen(cmd, stdout=Utils.subprocess.PIPE,
                                   cwd=submodule.abspath(),
                                   stderr=None, stdin=None)
        out = str(p.communicate()[0].strip())
        didGetVersion = (p.returncode == 0 and out != "")
        if didGetVersion:
            if out.startswith(tagPrefix):
                gitVersion = out[len(tagPrefix):]
            else:
                gitVersion = "%s-commit-%s" % (baseVersion, out)
    except OSError:
        pass

    versionFile = submodule.find_node('VERSION')

    if not didGetVersion and versionFile is not None:
        try:
            return splitVersion(versionFile.read())
        except (OSError, IOError):
            pass

    # version was obtained from git, update VERSION file if necessary
    if versionFile is not None:
        try:
            version = versionFile.read()
            versionFile = None # no need to update
        except (OSError, IOError):
            Logs.warn("VERSION file exists, but not readable")
    else:
        versionFile = submodule.make_node('VERSION')

    if versionFile:
        try:
            versionFile.write(gitVersion)
        except (OSError, IOError):
            Logs.warn("VERSION file is not writeable")

    return splitVersion(gitVersion)
