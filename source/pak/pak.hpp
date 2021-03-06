//
// Created by maros on 10/24/2020.
//

#ifndef ALICIACPAK_PAK_HPP
#define ALICIACPAK_PAK_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <csignal>
#include <unordered_map>
#include <string>
#include <zlib.h>
#include <fstream>
#include <filesystem>

#include "buffer/buffer.hpp"

/**
 * PAK Asset
 */
struct PakAsset {
    uint32_t prefix{}; // Always 0x0
    uint32_t magic{};

    uint32_t dataOffset{};
    uint32_t dataLength{};

    uint32_t uncompressedLength0{};
    uint32_t isCompressed{};
    uint32_t uncompressedLength1{};
    uint32_t unknown0{}; // Always 0x0
    uint32_t uncompressedLength2{};

    uint32_t unknown1{};
    uint32_t unknown2{};
    uint32_t unknown3{};
    uint32_t unknown4{};
    uint32_t unknown5{};
    uint32_t isDeleted{};
    uint32_t entryOffset{};
    uint32_t isPacked{};

    uint64_t unknownType{};
    uint64_t unknownValue{};

    uint32_t crcResult{};
    uint32_t unknown9{};

    uint32_t crcId{};
    uint32_t unknown7{};
    uint64_t unknown8{};


    std::wstring path;
    uint8_t *data{};

public:
    /***
     *
     * @return Standard cstr path to pak
     */
     char* GetStandardPath() {
        uint32_t length = path.length();
        if(length == 0)
            return nullptr;

        const wchar_t* cwPath = path.c_str();
        char *cPath = new char [length+1];

        for(uint32_t i = 0; i < length; i++) {
            // paths don't contain any widechars, we can just cast it and loose the most bits place
            cPath[i] = (char) cwPath[i];
        }
        cPath[length] = 0;

        return cPath;
    }

    bool HasPath() {
        return path[0] != 0x0;
     }
};

/**
 * Represents content of PAK
 *
 * too slow ;-;
 */
class PakContent {
private:
    std::unordered_map<std::wstring, PakAsset> assets;

public:
    /**
     * @return Asset count
     */
    uint64_t GetAssetCount() const {
        return assets.size();
    }

public:
    /**
     * Overload array operator
     * @param path Path of Asset
     * @return Asset
     */
    PakAsset operator[](const std::wstring& path) const {
        return this->assets.at(path);
    }

    /**
     * Overload plus operator
     * @param asset Asset to add
     * @return this
     */
    PakContent operator+=(PakAsset& asset) {
        this->assets[asset.path] = asset;
        return *this;
    }
};

/**
 * PAK File
 */
class PakFile {
private:
    FILE* fileHandle;
    bool isOk = false;

public:
    const static uint64_t ENTRY_SECTOR = 0x7D000;
    const static uint64_t DATA_SECTOR  = 0xF00000;

public:
    /**
     * Default constructor
     * @param path Path to PAK pak
     */
    explicit PakFile(const char* path);

    /**
     * Default deconstructor
     */
    ~PakFile();

public:
    /**
     * Loads PAK from disk
     */
    void Explore();

    /**
     * Destroys all data
     */
    void Destroy();

};


#endif //ALICIACPAK_PAK_HPP
