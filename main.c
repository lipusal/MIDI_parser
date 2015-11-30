#include <stdio.h>
#include <stdlib.h>
#include "MIDI.h"

int main(int argc, char** argv) {
    printf("Welcome to MIDI reader!");
    FILE *f = fopen("res/Super_Mario_All_Stars.mid", "rb");
//    FILE *f = fopen("res/Sim_City.mid", "rb");
    MIDI_header *h = loadHeader(f);
    printf("\n%s\n", headerToString(h));
    
    MIDI *m = newMIDI();
    m->header = h;
    m->tracks = malloc(sizeof(MIDI_track[m->header->num_tracks]));
    int i;
    for (i = 0; i < m->header->num_tracks; i++) {
        m->tracks[i] = loadTrackFromFILE(f);
        printf("Track #%d: %s\n", i+1, trackToString(m->tracks[i]));
    }
    
    return 0;
}

