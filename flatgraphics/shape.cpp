//
//  shape.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/23/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>
#include "gl.hpp"
#include <sstream>

#define PTS_PER_VERTEX 6

using std::cout;
using std::endl;

#define EXPORT __attribute__((visibility("default")))

/*
__attribute__((constructor))
static void initializer(void) {
	cout << "initializing " << __FILE__ << endl;
}

__attribute__((destructor))
static void finalizer(void) {
	cout << "finalizer " << __FILE__ << endl;
}*/

namespace shape {

	pt pt::operator+(pt p) {
		return {x + p.x, y + p.y};
	}
	pt pt::operator-(pt p) {
		return {x - p.x, y - p.y};
	}
	void pt::operator+=(pt p) {
		x += p.x;
		y += p.y;
	}
	void pt::operator-=(pt p) {
		x -= p.x;
		y -= p.y;
	}
	
	void fillData(float *f, float x, float y, const color *c) {
		f[0] = x;
		f[1] = y;
		f[2] = c->rf();
		f[3] = c->gf();
		f[4] = c->bf();
		f[5] = c->af();
	}
	
	
	geom::geom(GLuint renderMode, unsigned int size): renderMode(renderMode), size(size) {}
	
	geom::~geom() {
	}
	
	float* geom::genData() {
		float *f = new float[PTS_PER_VERTEX * size];
		genData(f);
		return f;
	}


	triangle::triangle(pt p1, pt p2, pt p3, const color *c): geom(GL_TRIANGLES, 3), c(c) {
		pts = new pt[3] {p1, p2, p3};
	}

	triangle::triangle(float x1, float y1, float x2, float y2, float x3, float y3, const util::color *c): triangle({x1, y1}, {x2, y2}, {x3, y3}, c) {}
	
//	triangle::triangle(triangle &t): geom(t.renderMode, t.size) {
//		// TODO
//	}

	triangle::~triangle() {
		delete [] pts;
	}
	
	void triangle::operator+=(pt p) {
		pts[0] += p;
		pts[1] += p;
		pts[2] += p;
	}
	void triangle::operator-=(pt p) {
		pts[0] -= p;
		pts[1] -= p;
		pts[2] -= p;
	}
	pt &triangle::operator[](const int i) {
		if (i < 0 || i > 2) {
			std::stringstream s;
			s << "Cannot acces element " << i << " of triangle";
			throw std::out_of_range(s.str());
		}
		return pts[i];
	}

	void triangle::genData(float *f) {
		fillData(f, pts[0].x, pts[0].y, c);
		fillData(f + PTS_PER_VERTEX, pts[1].x, pts[1].y, c);
		fillData(f + 2 * PTS_PER_VERTEX, pts[2].x, pts[2].y, c);
	}
	
	std::ostream &operator<<(std::ostream &o, triangle &t) {
		return o << "triangle ((" << t.pts[0].x << ", " << t.pts[0].y << "), (" << t.pts[1].x << ", " << t.pts[1].y << "), (" << t.pts[2].x << ", " << t.pts[2].y << "))";
	}
	
	
	conglomerate::conglomerate(int size, geom **geoms): geom(geoms[0]->renderMode, 0), geoms_len(size) {
		this->geoms = new pair<geom*, int>[size];
		int tot = 0;
		for (int i = 0; i < size; i++) {
			this->geoms[i].first = geoms[i];
			this->geoms[i].second = tot;
			tot += this->geoms[i].first->size;
		}
		this->size = tot;
	}
	
	conglomerate::conglomerate(vector<geom*> &geoms): geom(geoms[0]->renderMode, 0), geoms_len(int(geoms.size())) {
		this->geoms = new pair<geom*, int>[geoms_len];
		int tot = 0;
		for (int i = 0; i < geoms_len; i++) {
			this->geoms[i].first = geoms[i];
			this->geoms[i].second = tot;
			tot += this->geoms[i].first->size;
		}
		this->size = tot;
	}
	
	conglomerate::~conglomerate() {
		for (int i = 0; i < geoms_len; i++)
			delete geoms[i].first;
		delete [] geoms;
	}
	
	void conglomerate::genData(float *f) {
		for (int i = 0; i < geoms_len; i++) {
			pair<geom *, int> g = geoms[i];
			g.first->genData(f + PTS_PER_VERTEX * g.second);
		}
	}
	
	
	
	grid::grid(float x, float y, float width, float height, int rows, int cols): geom(GL_TRIANGLES, PTS_PER_VERTEX * rows * cols) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->rows = rows;
		this->cols = cols;
		this->colors = new const color*[rows * cols];
		for (int i = 0; i < rows * cols; i++)
			colors[i] = &util::black;
	}
	
	grid::~grid() {
		delete [] colors;
	}
	
	void grid::genData(float *f) {
		float dx = width / rows;
		float dy = height / cols;
		float xs = x - width / 2;
		float ys = y - height / 2;
		for (int j = 0; j < cols; j++) {
			float yp = ys + j * dy;
			for (int i = 0; i < rows; i++) {
				float xp = xs + i * dx;
				int ind = 36 * (i + rows * j);
				const color *c = colors[i + rows * j];
				fillData(f + ind, xp, yp, c);
				fillData(f + ind + 6, xp, yp + dy, c);
				fillData(f + ind + 12, xp + dx, yp, c);
				fillData(f + ind + 18, xp + dx, yp, c);
				fillData(f + ind + 24, xp, yp + dy, c);
				fillData(f + ind + 30, xp + dx, yp + dy, c);
			}
		}
	}
	
	void grid::fill(const color &c) {
		for (int i = 0; i < rows * cols; i++)
			colors[i] = &c;
	}
	
	void grid::set(int i, int j, const color *c) {
		colors[i + rows * j] = c;
	}
	
	void grid::setBorder(const color *c) {
		int a = rows * (cols - 1);
		for (int i = 0; i < rows; i++) {
			colors[i] = c;
			colors[i + a] = c;
		}
		for (int j = 0; j < cols; j++) {
			colors[j * rows] = c;
			colors[(j + 1) * rows - 1] = c;
		}
	}
	
	
	image::image(int width, int height, float screenWidth, float screenHeight, float centerX, float centerY): geom(GL_POINTS, PTS_PER_VERTEX * width * height), width(width), height(height), sw(screenWidth), sh(screenHeight), cx(centerX), cy(centerY) {
		this->colors = new const color*[width * height];
	}
	
	image::~image() {
		delete [] this->colors;
	}
	
	void image::genData(float *f) {
		int len = width * height;
		for (int i = 0; i < len; i++) {
			float x = (i % width) * sw / width - cx;
			float y = (i / width) * sh / height - cy;
			fillData(f + i * PTS_PER_VERTEX, x, y, colors[i]);
		}
	}
	
	void image::fill(const color &c) {
		int len = width * height;
		for (int i = 0; i < len; i++)
			colors[i] = &c;
	}
	
	void image::set(int i, int j, const color &c) {
		colors[i + j * width] = &c;
	}
	
	
	
	dynamicDrawer::dynamicDrawer(unsigned int capacity): geom(GL_TRIANGLES, capacity), pointCapacity(capacity * PTS_PER_VERTEX) {
		data = new float[pointCapacity];
		memset(data, 0, pointCapacity);
		off = 0;
	}
	
	dynamicDrawer::~dynamicDrawer() {
		delete [] data;
	}
	
	void dynamicDrawer::draw(geom *obj) {
		int size = obj->size * PTS_PER_VERTEX;
		if (off + size > pointCapacity) {
			cout << "Exceeded capacity of dynamicDrawer" << endl;
			return;
		}
		obj->genData(data + off);
		off += size;
	}
	
	void dynamicDrawer::reset() {
		memset(data, 0, pointCapacity);
		off = 0;
	}
	
	void dynamicDrawer::genData(float *f) {
		memcpy(f, data, pointCapacity * sizeof(float));
	}
	
	
	
}


namespace detail {
	
	void deref_buffers() {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void setup_vao() {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, PTS_PER_VERTEX * sizeof(GLfloat), (GLvoid *) 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, PTS_PER_VERTEX * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
	}
	
	renderable::renderable(geom* shape, bool dynamic, bool visible): dynamic(dynamic), draw_opt(dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW), render_mode(shape->renderMode) {
		this->shape = shape;
		this->visible = visible;
		
		// initialize to 0 by default, as these
		// are optional
		this->ebo = 0;
		this->numElements = 0;
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		setup_vao();
		
		float *temp = this->shape->genData();
		loadData(this->shape->size, temp);
		delete [] temp;
		
		deref_buffers();
	}
	
	renderable::~renderable() {
		if (ebo != 0)
			glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	
	void renderable::setVisible(bool v) {
		visible = v;
	}
	
	void renderable::draw(int offset) {
		if (!visible)
			return;
		if (dynamic) {
			float* temp = shape->genData();
			loadSubData(temp);
			delete [] temp;
		}
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		if (numElements != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDrawElements(render_mode, numElements, GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
			glDrawArrays(render_mode, 0, numVertices);
		
		deref_buffers();
	}
	
	// protected members
	void renderable::loadData(unsigned int size, float *data) {
		this->numVertices = size;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, PTS_PER_VERTEX * size * sizeof(float), data, draw_opt);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void renderable::loadElementBuffer(unsigned int size, int *data) {
		this->numElements = size;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), data, GL_STATIC_DRAW);
	}
	
	void renderable::loadSubData(float *data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, PTS_PER_VERTEX * numVertices * sizeof(float), data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
//	void renderable::change() const {
//		change_flag = true;
//	}
}


