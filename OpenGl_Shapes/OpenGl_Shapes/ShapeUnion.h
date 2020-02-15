#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include "Interfaces.h"
#include "BoundingBox.h"

class ShapeUnion : public ICanvasComponent {
private:
	std::vector<std::unique_ptr<ICanvasComponent>> _children;
	void forEach(std::function<void(std::unique_ptr<ICanvasComponent>&)>);
	BoundingBox calcBoundingBox() const override;

public:
	void Add(std::unique_ptr<ICanvasComponent>&);

	ISelectable* getById(int id) override;

	void draw() override;

	void setColor(const glm::vec4&) override;
	void setTrail(bool) override;
	void setDeformed(bool) override;
	void setScale(const glm::vec2&) override;
	void setHidden(bool) override;

	void translate(const glm::vec2&) override;
	void clampCanvasFit() override;
	
	void print(std::ostream& stream, std::string indent = "") const override;
};