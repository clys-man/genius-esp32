// Based on: https://github.com/robsoncouto/arduino-songs/
#include <Arduino.h>
#include <pitches.h>

class song
{
    private:
        int pin;
        void playSong(int melody[], int tempo, int notes);
    public:
        song(int pin);
        void playStartSong();
        void playLoseSong();
        void playWinSong();
        void playPointSong();
};

int buzzerPin;

song::song(int pin) {
    this->pin = pin;
    buzzerPin = pin;
}

void song::playSong(int melody[], int tempo, int notes) {
    int wholenote = (60000 * 4) / tempo;
    int divider = 0, noteDuration = 0;

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
        divider = melody[thisNote + 1];
        if (divider > 0) {
            noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
            noteDuration = (wholenote) / abs(divider);
            noteDuration *= 1.5; 
        }

        tone(buzzerPin, melody[thisNote], noteDuration * 0.9);
        delay(noteDuration);
        noTone(buzzerPin);
    }
}

void song::playStartSong() {
    int tempo = 120;

    int melody[] = {
        NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, 4, NOTE_D6, 4
    };

    int notes = sizeof(melody) / sizeof(melody[0]) / 2;

    playSong(melody, tempo, notes);
}

void song::playWinSong() {
    int tempo = 180;

    int melody[] = {
        NOTE_C4, 4, NOTE_G3, 8, NOTE_G3, 8, NOTE_A3, 4, NOTE_G3, 4, REST, 4, NOTE_B3, 4, NOTE_C4, 4
    };

    int notes = sizeof(melody) / sizeof(melody[0]) / 2;

    playSong(melody, tempo, notes);
}

void song::playLoseSong() {
    int tempo = 120;

    int melody[] = {
        NOTE_A5, 8, NOTE_B5, 8, NOTE_C6, 8, NOTE_D6, 8
    };

    int notes = sizeof(melody) / sizeof(melody[0]) / 2;

    playSong(melody, tempo, notes);
}
