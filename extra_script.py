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

def find_ui_c_and_remove_function():
    for dirpath, dnames, fnames in os.walk(os.path.join(PROJECT_DIR, "src")):
        for f in fnames:
            if f != "ui.c":
                continue
            file_path = os.path.join(dirpath, f)
            content = open(file_path, mode="r", encoding="utf-8").read()
            if re.search(r"void ui_init\(.*\)\s{[\s\S]+}\s?", content, re.MULTILINE):
                print("Remove ui_init function in {}".format(file_path))
                content = re.sub(r"void ui_init\(.*\)\s{[\s\S]+}\s?", "", content, re.MULTILINE) # remove ui_init function
                # print("New content: {}".format(content))
                open(file_path, mode="w", encoding="utf-8", newline='').write(content)

genheader()
find_ui_c_and_remove_function()
