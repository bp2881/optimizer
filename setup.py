import platform
from os import system

def windows():
    from windows import check_internet, check_update, remove_ai_sites
    # Check internet connection
    if not check_internet():
        print("No internet connection. Please check your connection and try again.")
        exit(1)
    
    # Check for updates
    if not check_update():
        pass

    # Remove AI sites
    remove_ai_sites()





# Check os
if platform.system() == 'Windows':
    # Setup for Windows
    system('winget install Git.Git -e --accept-source-agreements --accept-package-agreements')
    


    # Windows Section
    windows()

elif platform.system() == 'Darwin':
    from macos import check_internet, check_update, remove_ai_sites
