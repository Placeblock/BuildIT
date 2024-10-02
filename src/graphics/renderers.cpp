//
// Created by felix on 02.10.24.
//

#include "renderers.h"

#include "circuitBoard/components/cabling/wire.h"
#include "circuitBoard/components/cabling/joint.h"
#include "circuitBoard/components/nodes/notGate.h"
#include "circuitBoard/components/renderer/cablingRenderer.h"
#include "circuitBoard/components/renderer/node/notGateRenderer.h"

namespace TestRenderers {
    Renderers getRenderers(FontRenderer *fontRenderer) {
        ComponentRendererRegistry registry{};
        registry.registerRenderer([fontRenderer] {
            RendererData rendererData{};
            rendererData.types.insert(typeid(NotGate));
            rendererData.renderer = new NotGateRenderer{fontRenderer};
            return rendererData;
        });
        registry.registerRenderer([] {
            RendererData rendererData{};
            rendererData.types.insert(typeid(Wire));
            rendererData.types.insert(typeid(Joint));
            rendererData.renderer = new CablingRenderer{};
            return rendererData;
        });
        return registry.getNewRenderers();
    }
}