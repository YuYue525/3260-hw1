CSCI3260 Assignment 1 KeyBoard / Mouse Event

Name: YU Yue
SID: 1155124490

Manipulation:

	//Jellyfish
	Key "Esc": exit
	Key "s": Jellyfish moves backward
	Key "w": Jellyfish moves forward
	Key "a": Jellyfish moves left
	Key "d": Jellyfish moves right
	Key "q": Jellyfish becomes smaller (until 0.5 times)
	Key "e": Jellyfish becomes larger (until 3.0 times)

	//camerapos
	Key "↑(up)": camera position moves forward
	Key "↓(down)": camera position moves backward

	Dragging mouse (no press) to the left, right, up and down: rotate the camera accordingly slightly

	Dragging mouse (pressing left button) to the left: rotate the camera in clockwise direction of x-z plane
	Dragging mouse (pressing left button) to the right: rotate the camera in anti-clockwise direction of x-z plane
	Dragging mouse (pressing left button) to the upside:move the camera upward and still look at the objects
 	Dragging mouse (pressing left button) to the downside:move the camera downward and still look at the objects
	
	//perspective projection
	Scrolling the mouse wheel: field of view becomes larger or smaller accordingly

	//fishes
	There are several fishes in the scene and each one is rotating with different speeds over time

Parameters' effects of perspective projection:
fov: 
	with other parameters unchanged, when we increase the value of the field of view, in our window with constant size, the scene will become larger as if we are closing to the center of the object;
	when we decrease the value of the field of view, the scene will become smaller as if we are further away from the center of the object;
	actually, if we enlarge the field of view, we can see more objects around, but because of the constant size of the window, it seems that the object is further away, and vice versa.

	

