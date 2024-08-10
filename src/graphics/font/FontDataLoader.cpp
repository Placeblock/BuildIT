//
// Created by felix on 8/10/24.
//

#include <fstream>
#include <sstream>
#include "FontDataLoader.h"

void FontDataLoader::load() {
    std::ifstream infile(this->filePath);

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream lineStream(line);
        std::string action;
        lineStream >> action;
        if (action == "info") {
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                if (pair.first == "face") {
                    this->name = pair.second.substr(1, pair.second.length()-2);
                    continue;
                }
                converter << pair.second;
                if (pair.first == "size") {
                    converter >> this->size;
                } else if (pair.first == "bold") {
                    converter >> this->bold;
                } else if (pair.first == "italic") {
                    converter >> this->italic;
                }
            }
        } else if (action == "common") {
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "lineHeight") {
                    converter >> this->lineHeight;
                } else if (pair.first == "base") {
                    converter >> this->base;
                } else if (pair.first == "scaleW") {
                    converter >> this->bitmapSize.x;
                } else if (pair.first == "scaleH") {
                    converter >> this->bitmapSize.y;
                } else if (pair.first == "pages") {
                    converter >> this->pageCount;
                }
            }
        } else if (action == "page") {
            Page page{};
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "id") {
                    converter >> page.id;
                } else if (pair.first == "file") {
                    page.fileName = pair.second.substr(1, pair.second.length()-2);
                }
            }
            this->pages.push_back(page);
        } else if (action == "kerning") {
            Kerning kerning{};
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "first") {
                    converter >> kerning.firstID;
                } else if (pair.first == "second") {
                    converter >> kerning.secondID;
                } else if (pair.first == "amount") {
                    converter >> kerning.amount;
                }
            }
            this->kernings.push_back(kerning);
        } else if (action == "char") {
            Char newChar{};
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "id") {
                    converter >> newChar.id;
                } else if (pair.first == "x") {
                    converter >> newChar.pos.x;
                } else if (pair.first == "y") {
                    converter >> newChar.pos.y;
                } else if (pair.first == "width") {
                    converter >> newChar.size.x;
                } else if (pair.first == "height") {
                    converter >> newChar.size.y;
                } else if (pair.first == "xoffset") {
                    converter >> newChar.offset.x;
                } else if (pair.first == "yoffset") {
                    converter >> newChar.offset.y;
                } else if (pair.first == "xadvance") {
                    converter >> newChar.advance;
                } else if (pair.first == "page") {
                    converter >> newChar.page;
                }
            }
            this->chars[newChar.id] = newChar;
        }
    }
}

std::pair<std::string, std::string> FontDataLoader::readPair(std::istringstream* lineStream) {
    std::string pairString;
    *lineStream >> pairString;
    std::size_t i = pairString.find('=');
    std::string key = pairString.substr(0, i);
    std::string value = pairString.substr(i+1);
    return {key, value};
}
