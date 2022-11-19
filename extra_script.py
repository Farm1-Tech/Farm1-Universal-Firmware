# -*- coding: utf-8 -*-
import os
import re
import subprocess
Import("env")

PROJECT_DIR = env.subst("$PROJECT_DIR")

def genheader():
    revision = (
        subprocess.check_output(["git", "describe", "--tags", "--dirty", "--always"])
        .strip()
        .decode("utf-8")
    )
    content = ""
    content = content + "#define FW_VERSION \"%s\"\n" % revision
    open(os.path.join(PROJECT_DIR, "include/genheader.h"), mode="w", encoding="utf-8", newline='').write(content)

genheader()
