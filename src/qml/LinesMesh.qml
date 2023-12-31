import QtQuick
import QtQuick3D

Node {
	id: root

	property alias model: mesh.model

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
						diffuseColor: "lightgreen"
						lineWidth: 3
					}
				]
			}
		}
	}
}
