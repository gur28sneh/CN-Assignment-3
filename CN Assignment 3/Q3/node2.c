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
} dt2;

#define INFINITY 999
#define NODE_ID 2

int connectcosts2[4] = {3, 1, 0, 2};
int neighbors2[3] = {0, 1, 3};

// Function prototypes
void tolayer2(struct rtpkt packet);
void printdt2(struct distance_table *dtptr);

void send_vector2(int mincost[]) {
    struct rtpkt pkt;
    pkt.sourceid = NODE_ID;
    for (int i = 0; i < 3; i++) {
        pkt.destid = neighbors2[i];
        for (int j = 0; j < 4; j++) {
            pkt.mincost[j] = mincost[j];
        }
        tolayer2(pkt);
    }
}

void rtinit2() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt2.costs[i][j] = INFINITY;
        }
    }
    dt2.costs[0][0] = 3;
    dt2.costs[1][1] = 1;
    dt2.costs[2][2] = 0;
    dt2.costs[3][3] = 2;

    int mincost[4];
    for (int i = 0; i < 4; i++) {
        mincost[i] = connectcosts2[i];
    }

    printf("rtinit2() called at time %.3f\n", clocktime);
    printdt2(&dt2);
    send_vector2(mincost);
    printf("Node 2 sent initial vector to neighbors 0, 1, 3\n");
}

void rtupdate2(struct rtpkt *rcvdpkt) {
    int sender = rcvdpkt->sourceid;
    int updated = 0;
    int old_mincost[4], new_mincost[4];

    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt2.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt2.costs[i][j];
            }
        }
    }

    int cost_to_sender = connectcosts2[sender];
    for (int i = 0; i < 4; i++) {
        int new_cost = cost_to_sender + rcvdpkt->mincost[i];
        if (new_cost > INFINITY) new_cost = INFINITY;
        dt2.costs[i][sender] = new_cost;
    }

    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt2.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt2.costs[i][j];
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
            break;
        }
    }

    printf("rtupdate2() called at time %.3f\n", clocktime);
    printf("Received packet from node %d\n", sender);
    printdt2(&dt2);
    if (updated) {
        printf("Distance table updated, sending new vector\n");
        send_vector2(new_mincost);
    } else {
        printf("No update to distance table\n");
    }
}

void printdt2(struct distance_table *dtptr) {
    printf("                via     \n");
    printf("   D2 |    0     1    3 \n");
    printf("  ----|-----------------\n");
    printf("     0|  %3d   %3d   %3d\n", dtptr->costs[0][0],
           dtptr->costs[0][1], dtptr->costs[0][3]);
    printf("dest 1|  %3d   %3d   %3d\n", dtptr->costs[1][0],
           dtptr->costs[1][1], dtptr->costs[1][3]);
    printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][0],
           dtptr->costs[3][1], dtptr->costs[3][3]);
}