var underlayer = project.activeLayer;
var superlayer = new Layer();

class Node {
    constructor(position) {
        this.rect = new Path.Rectangle({ point: [-25, -25], size: [50, 50], fillColor: "white", strokeColor: "black" });
        this.rect.position = position;
        superlayer.addChild(this.rect);
    }
    get position() {
        return this.rect.position;
    }
    set position(value) {
        this.rect.position = value;
    }
}

class Edge {
    constructor(position, origin, target) {
        this.origin = origin;
        this.target = target;
        this.core = new Path.Circle({ center: position, radius: 10, fillColor: "white", strokeColor: "black" });
        superlayer.addChild(this.core);
        this.path = new Path({ strokeColor: "black" });
        underlayer.addChild(this.path);
        this.updatePath(origin, target);
    }
    get position() {
        return this.core.position;
    }
    set position(value) {
        this.core.position = value;
        this.updatePath()
    }
    updatePath() {
        this.path.add(this.origin.position);
        this.path.add(this.core.position);
        this.path.add(this.target.position);
        this.path.smooth();
    }
}

var nitrogen = new Node(new Point(100, 100));
var cylinder = new Node(new Point(200, 100));
var tank = new Node(new Point(300, 100));
var pipe2 = new Edge(new Point(150, 100), nitrogen, cylinder);
var pipe2 = new Edge(new Point(250, 100), cylinder, tank);
