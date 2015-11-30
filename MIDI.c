#include <stdlib.h>
#include <stdio.h>
#include <byteswap.h>   //MIDI files are big-endian, must flip ints
#include "MIDI.h"

//REFERENCE: http://www.ccarh.org/courses/253/handout/smf/
/*
 * TODO:
 *  -Validate proper MIDI format, i.e:
 *      -Header ID = "MThd"
 */

MIDI *newMIDI() {
    MIDI *m = malloc(sizeof(MIDI));
    if(m == NULL) return NULL;
    m->header = NULL;
    m->tracks = malloc(sizeof(MIDI_track *));
    return m;
}

MIDI_header *newHeader() {
    MIDI_header *h = malloc(sizeof(MIDI_header));
    if(h == NULL) return NULL;
    h->id = malloc(sizeof(uint8_t)*(4+1));
    h->id[4] = 0;       //Null terminator
    h->size = 0;
    h->format = 0;
    h->num_tracks = 0;
    h->timing = 0;
    return h;
}

MIDI_header *loadHeader(FILE *data) {
    MIDI_header *h = newHeader();
    if(h == NULL) return NULL;
    fgets(h->id, 5, data);                          //TODO validate === "MThd" -- reads (5-1) bytes
    fread(&(h->size), sizeof(uint32_t), 1, data);  //Should always be 6
    h->size = bswap_32(h->size);                  //Swap big-endian to little-endian, https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func/6961239#6961239
    fread(&(h->format), sizeof(uint16_t), 1, data);
    h->format = bswap_16(h->format);
    fread(&(h->num_tracks), sizeof(uint16_t), 1, data);
    h->num_tracks = bswap_16(h->num_tracks);
    fread(&(h->timing), sizeof(uint16_t), 1, data);
    h->timing = bswap_16(h->timing);
    return h;
}

MIDI_track *newTrack() {
    MIDI_track *t = malloc(sizeof(MIDI_track));
    if(t == NULL) return NULL;
    t->id = malloc(sizeof(uint8_t)*(4+1));
    t->id[4] = 0;       //Null terminator
    t->size = 0;
    t->data = NULL;
    return t;
}

MIDI_track * loadTrack(uint8_t *data) {
    MIDI_track *t = newTrack();
    if(t == NULL) return NULL;
    int i;
    for (i = 0; i < 4; i++) {
        t->id[i] = *data++;
    }
    t->size = (uint32_t) *data;
    data += 4;
    for(i = 0; i < t->size; i++) {
        t->data[i] = *data++;
    }
    return t;
}

MIDI_track * loadTrackFromFILE(FILE *data) {
    MIDI_track *t = newTrack();
    if(t == NULL) return NULL;
    fgets(t->id, 5, data);                          //TODO validate === "MTrk" -- reads (5-1) bytes
    fread(&(t->size), sizeof(uint32_t), 1, data);
    t->size = bswap_32(t->size);                    //Swap big-endian to little-endian, https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func/6961239#6961239
    t->data = malloc(t->size);
    fread(t->data, sizeof(uint8_t), t->size, data);
    return t;
}

char * headerToString(MIDI_header *h) {
    char *result;
    if(h == NULL) {
        result = "NULL header.";
    }
    else {
        result = malloc(125);
        char *fString;
        switch(h->format) {
            case 0:
                fString = "Single-track";
                break;
            case 1:
                fString = "Multiple-track";
                break;
            case 2:
                fString = "God help you";
                break;
        }
        uint32_t f = (uint32_t) h->format;
        uint32_t n = (uint32_t) h->num_tracks;
        int32_t div = (int32_t) h->timing;
        sprintf(result, "MIDI header:"
                "\n\tID: %s"
                "\n\tSize: %u"
                "\n\tFormat: %u (%s)"
                "\n\tTracks: %u"
                "\n\tTiming: %i", h->id, h->size, f, fString, n, div);
    }
    return result;
}

char * trackToString(MIDI_track *t) {
    char *result;
    if(t == NULL) {
        result = "NULL track.";
    }
    else {
        result = malloc(50);
        sprintf(result, "MIDI track:"
                "\n\tID: %s"
                "\n\tSize: %u", t->id, t->size);
    }
    return result;
}