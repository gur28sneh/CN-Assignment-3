#include <stdio.h>

extern struct rtpkt {
  int sourceid;
  int destid;
  int mincost[4];
};

extern int TRACE;
extern int YES;
extern int NO;
extern float clocktime;

struct distance_table {
  int costs[4][4];
} dt3;

#define INFINITY 999
#define NODE_ID 3

int connectcosts3[4] = {7, 999, 2, 0};
int neighbors3[2] = {0, 2};

void tolayer2(struct rtpkt packet);

void send_vector3(int mincost[]) {
    struct rtpkt pkt;
    pkt.sourceid = NODE_ID;
    for (int i = 0; i < 2; i++) {
        pkt.destid = neighbors3[i];
        for (int j = 0; j < 4; j++) {
            pkt.mincost[j] = mincost[j];
        }
        tolayer2(pkt);
    }
}

void rtinit3() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt3.costs[i][j] = INFINITY;
        }
    }
    dt3.costs[0][0] = 7;
    dt3.costs[2][2] = 2;
    dt3.costs[3][3] = 0;

    int mincost[4];
    for (int i = 0; i < 4; i++) {
        mincost[i] = connectcosts3[i];
    }

    printf("rtinit3() called at time %.3f\n", clocktime);
    printdt3(&dt3);
    send_vector3(mincost);
    printf("Node 3 sent initial vector to neighbors 0, 2\n");
}

void rtupdate3(struct rtpkt *rcvdpkt) {
    int sender = rcvdpkt->sourceid;
    int updated = 0;
    int old_mincost[4], new_mincost[4];

    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt3.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt3.costs[i][j];
            }
        }
    }

    int cost_to_sender = connectcosts3[sender];
    for (int i = 0; i < 4; i++) {
        int new_cost = cost_to_sender + rcvdpkt->mincost[i];
        if (new_cost > INFINITY) new_cost = INFINITY;
        dt3.costs[i][sender] = new_cost;
    }

    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt3.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt3.costs[i][j];
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
            break;
        }
    }

    printf("rtupdate3() called at time %.3f\n", clocktime);
    printf("Received packet from node %d\n", sender);
    printdt3(&dt3);
    if (updated) {
        printf("Distance table updated, sending new vector\n");
        send_vector3(new_mincost);
    } else {
        printf("No update to distance table\n");
    }
}

void printdt3(struct distance_table *dtptr) // Added definition
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
}