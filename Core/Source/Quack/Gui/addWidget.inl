// Text
template<std::same_as<Text> W>
void addWidget(std::string content) {
    W w;
    w.content = content;
    m_widgets.push_back(std::move(w));
}

// Button
template<std::same_as<Button> W>
void addWidget(std::string label, ImVec2 dim) {
    W w;
    w.label = label;
    w.dim = dim;
    w.event_callback = [this](Event& e) { raiseEvent(e); };
    m_widgets.push_back(std::move(w));
}

// SliderFloat
template<std::same_as<SliderFloat> W>
void addWidget(std::string label, ImVec2 minmax) {
    W w;
    w.label = label;
    w.minmax = minmax;
    w.value = minmax.x;
    w.event_callback = [this](Event& e) { raiseEvent(e); };
    m_widgets.push_back(std::move(w));
}

// Checkbox
template<std::same_as<Checkbox> W>
void addWidget(std::string label, bool initial_value) {
    W w;
    w.label = label;
    w.value = initial_value;
    w.event_callback = [this](Event& e) { raiseEvent(e); };
    m_widgets.push_back(std::move(w));
}