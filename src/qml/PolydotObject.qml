import QtQml
import QtQuick
import QtQuick3D

Node {
	id: root

	property QtObject model: null

	Repeater3D {
		model: root.model

		Loader3D {
			required property QtObject lineGeometry

			Model {
				scale: internal.scale
				geometry: lineGeometry
				materials: [
					DefaultMaterial {
						lighting: DefaultMaterial.NoLighting
						cullMode: DefaultMaterial.NoCulling
						diffuseColor: "green"
						lineWidth: 3
					}
				]
			}
		}
	}


	QtObject {
		id: internal

		readonly property vector3d scale: Qt.vector3d(100, 100, 100)
	}
}
