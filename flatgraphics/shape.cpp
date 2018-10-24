//
//  shape.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/23/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>
#include "gl.hpp"

#define PTS_PER_VERTEX 6


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
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		delete shape;
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
		glDrawArrays(render_mode, shape->offset, size);
		
		deref_buffers();
	}

	// protected members
	void renderable::loadData(unsigned int size, float *data) {
		this->size = size;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, PTS_PER_VERTEX * size * sizeof(float), data, draw_opt);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void renderable::loadSubData(float *data) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, PTS_PER_VERTEX * size * sizeof(float), data);
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

	void triangle::genData(float *f) {
		f[0] = pts[0].x;
		f[1] = pts[0].y;
		f[2] = c.rf();
		f[3] = c.gf();
		f[4] = c.bf();
		f[5] = c.af();
		f[6] = pts[1].x;
		f[7] = pts[1].y;
		f[8] = c.rf();
		f[9] = c.gf();
		f[10] = c.bf();
		f[11] = c.af();
		f[12] = pts[2].x;
		f[13] = pts[2].y;
		f[14] = c.rf();
		f[15] = c.gf();
		f[16] = c.bf();
		f[17] = c.af();
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
	
	
	
//	grid::grid(int rows, int cols, float width, float height, void *vals, map<void*, color> colorFunction): geom(GL_TRIANGLES, 3) {
//
//	}
	
}

