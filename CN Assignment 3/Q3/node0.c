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
} dt0;

#define INFINITY 999
#define NODE_ID 0

int connectcosts0[4] = {0, 1, 3, 7};
int neighbors0[3] = {1, 2, 3};

void tolayer2(struct rtpkt packet);

void send_vector0(int mincost[]) {
    struct rtpkt pkt;
    pkt.sourceid = NODE_ID;
    for (int i = 0; i < 3; i++) {
        pkt.destid = neighbors0[i];
        for (int j = 0; j < 4; j++) {
            pkt.mincost[j] = mincost[j];
        }
        tolayer2(pkt);
    }
}

void rtinit0() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt0.costs[i][j] = INFINITY;
        }
    }
    dt0.costs[0][0] = 0;
    dt0.costs[1][1] = 1;
    dt0.costs[2][2] = 3;
    dt0.costs[3][3] = 7;

    int mincost[4];
    for (int i = 0; i < 4; i++) {
        mincost[i] = connectcosts0[i];
    }

    printf("rtinit0() called at time %.3f\n", clocktime);
    printdt0(&dt0);
    send_vector0(mincost);
    printf("Node 0 sent initial vector to neighbors 1, 2, 3\n");
}

void rtupdate0(struct rtpkt *rcvdpkt) {
    int sender = rcvdpkt->sourceid;
    int updated = 0;
    int old_mincost[4], new_mincost[4];

    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt0.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt0.costs[i][j];
            }
        }
    }

    int cost_to_sender = connectcosts0[sender];
    for (int i = 0; i < 4; i++) {
        int new_cost = cost_to_sender + rcvdpkt->mincost[i];
        if (new_cost > INFINITY) new_cost = INFINITY;
        dt0.costs[i][sender] = new_cost;
    }

    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt0.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt0.costs[i][j];
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
            break;
        }
    }

    printf("rtupdate0() called at time %.3f\n", clocktime);
    printf("Received packet from node %d\n", sender);
    printdt0(&dt0);
    if (updated) {
        printf("Distance table updated, sending new vector\n");
        send_vector0(new_mincost);
    } else {
        printf("No update to distance table\n");
    }
}

void printdt0(struct distance_table *dtptr) {
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
         dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
         dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
         dtptr->costs[3][2],dtptr->costs[3][3]);
}

void linkhandler0(int linkid, int newcost) {
    printf("\nlinkhandler0() called at time %.3f\n", clocktime);
    printf("Link cost between %d and %d changed to %d\n", NODE_ID, linkid, newcost);

    int updated = 0;
    int old_mincost[4], new_mincost[4];

    // Store old minimum costs
    for (int i = 0; i < 4; i++) {
        old_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt0.costs[i][j] < old_mincost[i]) {
                old_mincost[i] = dt0.costs[i][j];
            }
        }
    }

    // Update the direct cost
    connectcosts0[linkid] = newcost;
    dt0.costs[linkid][linkid] = newcost;

    // Recompute all paths that might use this link
    for (int i = 0; i < 4; i++) {
        if (i == NODE_ID || i == linkid) continue;
        int new_cost = newcost + dt0.costs[i][linkid];
        if (new_cost < dt0.costs[i][linkid]) {
            dt0.costs[i][linkid] = new_cost;
        }
    }

    // Compute new minimum costs
    for (int i = 0; i < 4; i++) {
        new_mincost[i] = INFINITY;
        for (int j = 0; j < 4; j++) {
            if (dt0.costs[i][j] < new_mincost[i]) {
                new_mincost[i] = dt0.costs[i][j];
            }
        }
        if (new_mincost[i] != old_mincost[i]) {
            updated = 1;
        }
    }

    printdt0(&dt0);
    if (updated) {
        printf("Link change caused update, sending new vector\n");
        send_vector0(new_mincost);
    } else {
        printf("Link change didn't affect distance table\n");
    }
}