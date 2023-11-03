import QtQuick
import QtQuick3D

Node {
	id: root

	property alias model: mesh.model

	Repeater3D {
		id: mesh

		Loader3D {
			required property QtObject lineGeometry

			asynchronous: true

			Model {
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
}
