#include "main.h"
using namespace Engine;

bool initialized = false;
std::vector<SoundBuffer*> sounds;
std::vector<pSoundInstance> instances;

SoundBuffer* get_sound(size_t id) {
    if(id >= sounds.size()) return nullptr;
    return sounds[id];
}

pSoundInstance get_instance(size_t id){
    if(id >= instances.size()) return nullptr;
    return instances[id];
}


extern "C" {

    GMEXPORT double dll_init() {
		if(initialized) return 0;
		
        sounds.reserve(1024 * 64);
        instances.reserve(1024 * 64);
		
		initialized = true;
        return 1;
    }

    GMEXPORT double dll_free() {
		if(!initialized) return 0;
        for(SoundInstance* ii : instances){
            if(ii == nullptr) continue;
            delete ii;
        }
        for(SoundBuffer* buf : sounds){
            if(buf == nullptr) continue;
            delete buf;
        }
        instances.clear();
        sounds.clear();
        AudioContext::current().free();

		initialized = false;
        return 1;
    }

    GMEXPORT double clean_memory(double destroyInstances) {
		if(!initialized) return -1;
        size_t count = 0;
        if(destroyInstances > 0){ // free sound instances that are not playing
            for(SoundInstance*& ii : instances){
                if(ii == nullptr) continue;
                if(!ii->isPlaying()){
                    delete ii;
                    ii = nullptr;
                    count++;
                }
            }
        }
        { // clean up empty trailing null pointers
            size_t pos;
            for(pos = instances.size()-1; pos >= 0; --pos){
                if(instances[pos] != nullptr) break;
            }
            ++pos; // do not remove the final instance
            if(pos < instances.size()) instances.erase(instances.begin() + pos, instances.end());

            for(pos = sounds.size()-1; pos >= 0; --pos){
                if(sounds[pos] != nullptr) break;
            }
            ++pos; // do not remove the final instance
            if(pos < sounds.size()) sounds.erase(sounds.begin() + pos, sounds.end());
        }

        return count;
    }

    GMEXPORT double sound_add(const char* path) {
		if(!initialized) return -1;
        SoundBuffer* snd = new SoundBuffer;

        if(!snd->loadOGGFromFile(path)) {
            delete snd;
            snd = nullptr;
            return -1;
        }

        size_t id = sounds.size();
        sounds.push_back(snd);

        return id;
    }

    GMEXPORT double sound_remove(double id) {
        SoundBuffer* snd = get_sound(id);
        if(snd == nullptr) return 0;

        for(SoundInstance*& ii : instances){
            if(ii == nullptr) continue;
            if(ii->fromSoundBuffer(snd)){
                delete ii;
                ii = nullptr;
            }
        }
        sounds[id] = nullptr;

        return 1;
    }

    GMEXPORT double sound_play(double id) {
        SoundBuffer* snd = get_sound(id);

        if(snd == nullptr) return -1;

        pSoundInstance ii = snd->play();

        size_t _id = instances.size();
        instances.push_back(ii);

        return _id;
    }

    GMEXPORT double sound_play_ext(double id, double loop, double volume, double position, double speed) {
        SoundBuffer* snd = get_sound(id);

        if(snd == nullptr) return -1;

        pSoundInstance ii = snd->create();

        size_t _id = instances.size();
        instances.push_back(ii);

        ii->position(position);
        ii->volume(volume);
        ii->setspeed(speed);

        if(loop > 0){
            ii->loop();
        } else {
            ii->play();
        }

        return _id;
    }

    GMEXPORT double sound_length(double id) {
        SoundBuffer* snd = get_sound(id);

        if(snd == nullptr) return -1;

        return snd->length();
    }

    GMEXPORT double ii_destroy(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;

        instances[id] = nullptr;
        delete ii;

        return 1;
    }

    GMEXPORT double ii_pause(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        ii->pause();

        return 1;
    }

    GMEXPORT double ii_resume(double id, double loop) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        if(loop > 0){
            ii->loop();
        } else {
            ii->play();
        }

        return 1;
    }

    GMEXPORT double ii_set_speed(double id, double speed) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        ii->setspeed(speed);

        return 1;
    }

    GMEXPORT double ii_get_speed(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        return ii->getspeed();
    }

    GMEXPORT double ii_set_position(double id, double position) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        ii->position(position);

        return 1;
    }

    GMEXPORT double ii_get_position(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        return ii->position();
    }

    GMEXPORT double ii_set_volume(double id, double volume) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        ii->volume(volume);

        return 1;
    }

    GMEXPORT double ii_get_volume(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        return ii->volume();
    }

    GMEXPORT double ii_get_sound(double id) {
        pSoundInstance ii = get_instance(id);

        if(ii == nullptr) return -1;
        SoundBuffer* snd = ii->getSoundBuffer();

        if(snd == nullptr) return -1;
        auto pos = std::find(sounds.begin(), sounds.end(), snd);

        return double(pos - sounds.begin());
    }

}
