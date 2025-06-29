#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define HOSTS_FILE_PATH "C:\\Windows\\System32\\drivers\\etc\\hosts"
    #define BACKUP_FILE_PATH "C:\\Windows\\System32\\drivers\\etc\\hosts.backup"
#else
    #define HOSTS_FILE_PATH "/etc/hosts"
    #define BACKUP_FILE_PATH "/etc/hosts.backup"
#endif

const char *entries[] = {
    "127.0.0.1 bard.google.com",
    "127.0.0.1 gemini.google.com",
    "127.0.0.1 poe.com",
    "127.0.0.1 chat.bing.com",
    "127.0.0.1 perplexity.ai",
    "127.0.0.1 you.com",
    "127.0.0.1 claude.ai",
    "127.0.0.1 chat.anthropic.com",
    "127.0.0.1 huggingface.co",
    "127.0.0.1 beta.character.ai",
    "127.0.0.1 character.ai",
    "127.0.0.1 c.ai",
    "127.0.0.1 chatgptfree.ai",
    "127.0.0.1 chatgpt.org",
    "127.0.0.1 gptgo.ai",
    "127.0.0.1 open-assistant.io",
    "127.0.0.1 writesonic.com",
    "127.0.0.1 chatsonic.com",
    "127.0.0.1 janitorai.com",
    "127.0.0.1 chagpt.org",
    "127.0.0.1 koboldai.net",
    "127.0.0.1 aitoolmall.com",
    "127.0.0.1 nat.dev",
    "127.0.0.1 api.writesonic.com",
	"127.0.0.1 chatgpt.com",
	"127.0.0.1 chat.deepseek.com"
};

int backup_hosts_file() {
    FILE *src = fopen(HOSTS_FILE_PATH, "r");
    FILE *dest = fopen(BACKUP_FILE_PATH, "w");
    if (src == NULL || dest == NULL) {
        perror("Error backing up hosts file. Are you running with proper permissions?");
        return 1;
    }

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
    return 0;
}

int main() {
    printf("Backing up original hosts file to: %s\n", BACKUP_FILE_PATH);
    if (backup_hosts_file() != 0) {
        return 1;
    }

    FILE *file = fopen(HOSTS_FILE_PATH, "a");
    if (file == NULL) {
        perror("Error opening hosts file for writing. Run as administrator/root.");
        return 1;
    }

    fprintf(file, "\n# Block AI Chat Tools\n");
    for (int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
        fprintf(file, "%s\n", entries[i]);
    }

    fclose(file);
    printf("AI sites successfully added to hosts file.\n");
    return 0;
}
