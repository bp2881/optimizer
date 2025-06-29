import platform
from ping3 import ping
from os import system


def check_internet():
    response = ping("google.com")
    if response != False:
        return True

    return response


def remove_ai_sites():
    if (check_internet()) == False:
        return False
    # Get remove_ai.exe
    system('curl -O https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/remove_ai_sites.exe')
    system('curl -O https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/remove_ai_sites.exe.manifest')
    system('remove_ai_sites.exe')

    # remove files
    system('powershell rm remove_ai_*')

# Check os
if platform.system() == 'Windows':
    
    # Setup for Windows
    check_internet()
    remove_ai_sites()

    ## Create Optimizer directory
    system('mkdir "C:\\Program Files\\Optimizer"')
    ## Copy version.json to Optimizer directory
    system('curl -o "C:\\Program Files\\Optimizer\\version.json" https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/version.json')
    

