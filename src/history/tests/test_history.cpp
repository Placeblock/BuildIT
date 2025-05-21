//
// Created by felix on 08.05.25.
//

struct TestComponent {
    int32_t value;

    bool operator==(const TestComponent& rhs) const {
        return value == rhs.value;
    }
};

int main() {
    
}
