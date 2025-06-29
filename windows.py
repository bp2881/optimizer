from ping3 import ping
from os import system


def check_internet():
    response = ping("google.com")
    if response != False:
        return True

    return response
    

def check_update():
    if (check_internet()) == False:
        return False
    response = ping("google.com")
    if response != False:
        # Update
        pass


def remove_ai_sites():
    if (check_internet()) == False:
        return False
    # Get remove_ai.exe
    system('curl -O https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/remove_ai_sites.exe')
    system('curl -O https://raw.githubusercontent.com/bp2881/optimizer/refs/heads/main/remove_ai_sites.exe.manifest')
    system('remove_ai_sites.exe')

    # remove files
    system('powershell rm remove_ai_*')


