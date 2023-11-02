import QtQml
import QtQuick
import QtQuick3D
import PolydotTransformationUi

Node {
	id: root

	property alias text: label.text
	property alias pos: geom.point
	property alias geom: geom
	property alias pointSize: material.pointSize

	Model {
		id: basisPoint

		scale: internal.scale
		geometry: PointGeometry {
			id: geom
		}
		materials: [
			DefaultMaterial {
				id: material

				lighting: DefaultMaterial.NoLighting
				cullMode: DefaultMaterial.NoCulling
				diffuseColor: "lightblue"
				pointSize: sliderPointSize.value * 100
				lineWidth: pointSize
			}
		]
	}

	Node {
		x: (root.pos.x) * internal.scale.x
		y: (root.pos.y + 0.2) * internal.scale.y

		Text {
			id: label

			anchors.centerIn: parent
			font.pixelSize: 16
			color: "#e0e0e0"
			style: Text.Raised
		}
	}

	QtObject {
		id: internal

		readonly property vector3d scale: Qt.vector3d(100, 100, 100)
	}
}
