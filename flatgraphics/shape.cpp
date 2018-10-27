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
	
	void fill(float *f, float x, float y, color c) {
		f[0] = x;
		f[1] = y;
		f[2] = c.rf();
		f[3] = c.gf();
		f[4] = c.bf();
		f[5] = c.af();
	}
	
	
	geom::geom(GLuint renderMode, unsigned int size): renderMode(renderMode), size(size), offset(0) {}
	
	geom::~geom() {
	}
	
	float* geom::genData() {
		float *f = new float[PTS_PER_VERTEX * size];
		genData(f);
		return f;
	}


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
		change_flag = false;
		
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

	void renderable::draw() {
		if (!visible)
			return;
		if (change_flag) {
			float* temp = shape->genData();
			loadSubData(temp);
			delete [] temp;
			
			change_flag = false;
		}
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		if (numElements != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDrawElements(render_mode, numElements, GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
			glDrawArrays(render_mode, shape->offset, numVertices);
		
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

	void renderable::change() const {
		change_flag = true;
	}


	triangle::triangle(pt p1, pt p2, pt p3, color c): geom(GL_TRIANGLES, 3), c(c) {
		pts = new pt[3] {p1, p2, p3};
	}

	triangle::triangle(float x1, float y1, float x2, float y2, float x3, float y3, util::color c): triangle({x1, y1}, {x2, y2}, {x3, y3}, c) {}
	
	triangle::triangle(triangle &t): geom(t.renderMode, t.size) {
		// TODO
	}

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
		fill(f, pts[0].x, pts[0].y, c);
		fill(f + 6, pts[1].x, pts[1].y, c);
		fill(f + 12, pts[2].x, pts[2].y, c);
	}
	
	std::ostream &operator<<(std::ostream &o, triangle &t) {
		return o << "triangle ((" << t.pts[0].x << ", " << t.pts[0].y << "), (" << t.pts[1].x << ", " << t.pts[1].y << "), (" << t.pts[2].x << ", " << t.pts[2].y << "))";
	}
	
	
	int totalSize(int size, geom **geoms) {
		int tot = 0;
		for (int i = 0; i < size; i++) {
			// updates their offsets while totaling
			geoms[i]->offset = tot;
			tot += geoms[i]->size;
		}
		return tot;
	}
	
	conglomerate::conglomerate(int size, geom **&geoms): geom(geoms[0]->renderMode, totalSize(size, geoms)), geoms_len(size) {
		this->geoms = geoms;
		// don't use old geoms after making this
		geoms = NULL;
	}
	
	conglomerate::conglomerate(vector<geom*> &geoms): geom(geoms[0]->renderMode, totalSize(int(geoms.size()), &(geoms[0]))), geoms_len(int(geoms.size())) {
		this->geoms = new geom*[geoms_len];
		for (int i = 0; i < geoms_len; i++)
			this->geoms[i] = geoms[i];
	}
	
	conglomerate::~conglomerate() {
		for (int i = 0; i < geoms_len; i++)
			delete geoms[i];
		delete [] geoms;
	}
	
	void conglomerate::genData(float *f) {
		for (int i = 0; i < geoms_len; i++) {
			geom *&g = geoms[i];
			g->genData(f + PTS_PER_VERTEX * g->offset);
		}
	}
	
	
	
	grid::grid(float x, float y, float width, float height, int rows, int cols, colorFunc colorFunction): geom(GL_TRIANGLES, 6 * rows * cols) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->rows = rows;
		this->cols = cols;
		this->colorFunction = colorFunction;
	}
	
	grid::~grid() {}
	
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
				color c = colorFunction(i, j);
				fill(f + ind, xp, yp, c);
				fill(f + ind + 6, xp, yp + dy, c);
				fill(f + ind + 12, xp + dx, yp, c);
				fill(f + ind + 18, xp + dx, yp, c);
				fill(f + ind + 24, xp, yp + dy, c);
				fill(f + ind + 30, xp + dx, yp + dy, c);
			}
		}
	}
	
}

