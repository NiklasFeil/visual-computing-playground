// Text
template<std::same_as<Text> W>
void addWidget(std::string label, std::string text) {
    W t;
    t.text = text;

    Widget w;
    w.data = std::move(t);
    w.label = std::move(label);
    m_widgets.push_back(std::move(w));
}

// Button
template<std::same_as<Button> W>
void addWidget(std::string label, std::string text, ImVec2 dim) {
    W b;
    b.text = text;
    b.dim = dim;
    b.event_callback = [this](Event& e) { raiseEvent(e); };

    Widget w;
    w.data = std::move(b);
    w.label = std::move(label);
    m_widgets.push_back(std::move(w));
}

// SliderFloat
template<std::same_as<SliderFloat> W>
void addWidget(std::string label, std::string text, ImVec2 minmax, float initial_value) {
    W s;
    s.text = text;
    s.minmax = minmax;
    s.value = initial_value;
    s.event_callback = [this](Event& e) { raiseEvent(e); };

    Widget w;
    w.data = std::move(s);
    w.label = std::move(label);
    m_widgets.push_back(std::move(w));
}

// Checkbox
template<std::same_as<Checkbox> W>
void addWidget(std::string label, std::string text, bool initial_value) {
    W c;
    c.text = text;
    c.value = initial_value;
    c.event_callback = [this](Event& e) { raiseEvent(e); };
    
    Widget w;
    w.data = std::move(c);    
    w.label = std::move(label);
    m_widgets.push_back(std::move(w));
}