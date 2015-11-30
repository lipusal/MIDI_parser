#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

typedef struct {
    uint8_t *id;    //Should always be 4 characters long
    uint32_t size;
    uint8_t *data;
} MIDI_track;

typedef struct {
    uint8_t *id;    //Should always be 4 characters long
    uint32_t size;
    uint16_t format;
    uint16_t num_tracks;
    int16_t timing;
} MIDI_header;

typedef struct {
    MIDI_header *header;
    MIDI_track **tracks;
} MIDI;

MIDI *newMIDI();

MIDI_header *newHeader();

MIDI_header *loadHeader(FILE *data);

MIDI_track *newTrack();

MIDI_track * loadTrack(uint8_t *data);

MIDI_track * loadTrackFromFILE(FILE *data);

char * trackToString(MIDI_track *t);

char * headerToString(MIDI_header *t);

#endif /* MIDI_H */

