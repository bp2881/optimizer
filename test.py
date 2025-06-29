import json

with open("file1.json") as f1, open("file2.json") as f2:
    old_data = json.load(f1)["app_version"]
    new_data = json.load(f2)["app_version"]

for app, old_version in old_data.items():
    if app not in new_data:
        print(f"{app}: missing in new file (version {old_version})  # update this app")
    elif new_data[app] != old_version:
        print(f"{app}: different versions (old: {old_version}, new: {new_data[app]})")