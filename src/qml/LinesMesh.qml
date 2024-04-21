import QtQuick
import QtQuick3D

Node {
	id: root

	property alias model: mesh.model
	property color color: "black"

	Repeater3D {
		id: mesh

		delegate: Loader3D {
			required property QtObject lineGeometry

			asynchronous: true

			Model {
				geometry: lineGeometry
				materials: [
					DefaultMaterial {
						lighting: DefaultMaterial.NoLighting
						cullMode: DefaultMaterial.NoCulling
						diffuseColor: root.color
						lineWidth: 3
					}
				]
			}
		}
	}
}
