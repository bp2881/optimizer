import sys
import json
import requests
from ping3 import ping
import os

VERSION_PATH = "C:\\Program Files\\Optimizer\\version.json"
FOLDER_PATH = "C:\\Program Files\\Optimizer"
REMOTE_VERSION_URL = "https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/version.json"
TEMP_NEW_VERSION = "new_version.json"

def check_internet():
    return ping("google.com") is not False

def download_file(url, dest_path):
    try:
        response = requests.get(url)
        if response.status_code == 200:
            with open(dest_path, 'wb') as f:
                f.write(response.content)
            return True
        else:
            print(f"Failed to download {url} (status code: {response.status_code})")
            return False
    except Exception as e:
        print(f"Error downloading {url}: {e}")
        return False

def download_app(app, app_url_dict):
    updated = False

    if app in app_url_dict:
        url = app_url_dict[app]
        filename = url.split('/')[-1]
        dest_path = os.path.join(FOLDER_PATH, filename)
        if download_file(url, dest_path):
            print(f"{app}: downloaded {filename}")
            updated = True
    else:
        print(f"{app}: no URL provided, skipping main download")

    # Download manifest if available
    manifest_key = f"{app}_manifest"
    if manifest_key in app_url_dict:
        manifest_url = app_url_dict[manifest_key]
        manifest_filename = manifest_url.split('/')[-1]
        manifest_path = os.path.join(FOLDER_PATH, manifest_filename)
        if download_file(manifest_url, manifest_path):
            print(f"{app}: downloaded manifest {manifest_filename}")

    return updated

def check_update():
    if not check_internet():
        print("No internet connection. Cannot check for updates.")
        return

    if not download_file(REMOTE_VERSION_URL, TEMP_NEW_VERSION):
        print("Failed to fetch new version info.")
        return

    with open(VERSION_PATH) as f1, open(TEMP_NEW_VERSION) as f2:
        old_data = json.load(f1)
        new_data = json.load(f2)

    old_versions = old_data.get("app_version", {})
    new_versions = new_data.get("app_version", {})
    app_urls = new_data.get("app_url", {})

    updated = False

    for app, old_version in old_versions.items():
        if app not in new_versions or new_versions[app] != old_version:
            print(f"{app}: missing in new file (version {old_version})  # update this app")
            if download_app(app, app_urls):
                updated = True
                ## Move the rmd to startup
    if updated:
        with open(VERSION_PATH, 'w') as f:
            json.dump(new_data, f, indent=4)
        print("Local version.json updated.")
    else:
        print("Nothing to update.")

def get_version():
    try:
        response = requests.get(REMOTE_VERSION_URL)
        if response.ok:
            version = response.json()
            print(f"Optimizer Version: {version['version']}")
        else:
            print("Failed to fetch version.")
    except Exception as e:
        print(f"Error fetching version: {e}")

def main():
    arg1 = sys.argv[1] if len(sys.argv) > 1 else None

    match arg1:
        case "-v" | "--version":
            get_version()
        case "-u" | "--update":
            check_update()
        case default:
            get_version()

if __name__ == "__main__":
    main()
