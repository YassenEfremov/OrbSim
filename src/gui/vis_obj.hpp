#ifndef VIS_OBJ_HPP
#define VIS_OBJ_HPP


class VisObj {

public:
	virtual ~VisObj() = default;

	virtual void create() = 0;
	virtual void render() = 0;
};

#endif	// VIS_OBJ_HPP

