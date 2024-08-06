//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_APPLICATION_H
#define BUILDIT_APPLICATION_H


#include "graphics/data/eventHandler.h"
#include "graphics/data/graphicData.h"
#include "programs.h"
#include "graphics/data/renderer.h"

class Application : public EventHandler, public Renderer {
private:
    GraphicData graphicData;
    Programs programs;
public:
    explicit Application(GraphicData data) : graphicData(data), programs(Programs(&this->graphicData)) {};
    void onResize(int width, int height) override;
    void onScroll(double xOffset, double yOffset) override;
    void onKeyAction(int key, int scanCode, int action, int mods) override;
    void onMouseAction(int button, int action, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
    [[nodiscard]] glm::vec2 getMousePos() const;

    void render() override;
};


#endif //BUILDIT_APPLICATION_H
