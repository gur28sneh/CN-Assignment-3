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

int connectcosts1[4] = {1, 0, 1, 999};
struct distance_table {
  int costs[4][4];
} dt1;

#define INFINITY 999
#define NODE_ID 1

int neighbors1[2] = {0, 2};

void tolayer2(struct rtpkt packet);

void send_vector1(int mincost[]) {
    struct rtpkt pkt;
    pkt.sourceid = NODE_ID;
    for (int i = 0; i < 2; i++) {
        pkt.destid = neighbors1[i];
        for (int j = 0; j < 4; j++) {
            pkt.mincost[j] = mincost[j];
        }
        tolayer2(pkt);
    }
}

void rtinit1() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt1.costs[i][j] = INFINITY;
        }
    }
    dt1.costs[0][0] = 1;
    dt1.costs[1][1] = 0;
    dt1.costs[2][2] = 1;

    int mincost[4];
    for (int i = 0; i < 4; i++) {
        mincost[i] = connectcosts1[i];
    }

    printf("rtinit1() called at time %.3f\n", clocktime);
    printdt1(&dt1);
    send_vector1(mincost);
    printf("Node 1 sent initial vector to neighbors 0, 2\n");
}

void rtupdate1(struct rtpkt *rcvdpkt) {
    int sender = rcvdpkt->sourceid;
    int updated = 0;
    int old_mincost[4], new_mincost[4];

    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt1.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt1.costs[i][j];
            }
        }
    }

    int cost_to_sender = connectcosts1[sender];
    for (int i = 0; i < 4; i++) {
        int new_cost = cost_to_sender + rcvdpkt->mincost[i];
        if (new_cost > INFINITY) new_cost = INFINITY;
        dt1.costs[i][sender] = new_cost;
    }

    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt1.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt1.costs[i][j];
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
            break;
        }
    }

    printf("rtupdate1() called at time %.3f\n", clocktime);
    printf("Received packet from node %d\n", sender);
    printdt1(&dt1);
    if (updated) {
        printf("Distance table updated, sending new vector\n");
        send_vector1(new_mincost);
    } else {
        printf("No update to distance table\n");
    }
}

void printdt1(struct distance_table *dtptr) {
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);
}

void linkhandler1(int linkid, int newcost) {
    printf("\nlinkhandler1() called at time %.3f\n", clocktime);
    printf("Link cost between %d and %d changed to %d\n", NODE_ID, linkid, newcost);

    int updated = 0;
    int old_mincost[4], new_mincost[4];

    // Store old minimum costs
    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt1.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt1.costs[i][j];
            }
        }
    }

    // Update the direct cost
    connectcosts1[linkid] = newcost;
    dt1.costs[linkid][linkid] = newcost;

    // Recompute all paths that might use this link
    for (int i = 0; i < 4; i++) {
        if (i == NODE_ID || i == linkid) continue;
        int new_cost = newcost + dt1.costs[i][linkid];
        if (new_cost < dt1.costs[i][linkid]) {
            dt1.costs[i][linkid] = new_cost;
        }
    }

    // Compute new minimum costs
    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt1.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt1.costs[i][j];
            }
        }
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
        }
    }

    printdt1(&dt1);
    if (updated) {
        printf("Link change caused update, sending new vector\n");
        send_vector1(new_mincost);
    } else {
        printf("Link change didn't affect distance table\n");
    }
}