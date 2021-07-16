function moveObject(obj, move_direction, left, right) {
	var moveAmount = 40;
	obj.addToX(moveAmount * move_direction);
	if(obj.xPos() <= left - moveAmount) {
		obj.getMoveComponent().setMovementDirection(1);
		obj.addToY(30);
		obj.addToX(moveAmount * 1);
	}
	else if(obj.xPos() >= right + moveAmount) {
		obj.getMoveComponent().setMovementDirection(-1);
		obj.addToY(30);
		obj.addToX(moveAmount * -1);
	}
	return "Script Success";
}