#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sysexits.h>

#include "memagent.h"

void display_lists(void* userdata, memcached_server_list_t** lists)
{
    int i = 0;

    printf("Hey.  I received a new list of server lists (userdata: %s):\n",
           (char*)userdata);

    for (i = 0; lists[i]; i++) {
        int j = 0;
        memcached_server_list_t* list = lists[i];
        printf("\tList:  ``%s'' (bound to %d)\n", list->name, list->binding);

        for (j = 0; list->servers[j]; j++) {
            memcached_server_t* server = list->servers[j];
            printf("\t\t%s:%d\n", server->host, server->port);
        }
    }
}

void do_stats(void* userdata, void* opaque, agent_add_stat add_stat)
{
    add_stat(opaque, "stat1", "val1");
    add_stat(opaque, "stat2", "val2");
    add_stat(opaque, NULL, NULL);
}

int main(int argc, char **argv) {

    agent_config_t conf;

    if (argc < 3) {
        fprintf(stderr, "Usage: bot <jid> <pass> [host]\n");
        exit(EX_USAGE);
    }

    conf.jid = argv[1];
    conf.pass = argv[2];
    conf.host = (argc == 4 ? argv[3] : NULL);
    conf.software = "agent sample bot";
    conf.version = "1.0";
    conf.save_path = "/tmp/memagent.db";

    conf.userdata = "something awesome";
    conf.new_serverlist = display_lists;
    conf.get_stats = do_stats;

    if(!start_agent(conf)) {
        fprintf(stderr, "Couldn't initialize the agent.\n");
        exit(1);
    }

    pause();
}
