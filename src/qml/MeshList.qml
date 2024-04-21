import QtQuick
import QtQuick3D

Node {
	id: root

	property alias model: meshes.model
	property color color: "black"

	Repeater3D {
		id: meshes

		delegate: Loader3D {
			//required property QtObject linesMeshModel

			asynchronous: true

			LinesMesh {
				model: linesMeshModel
				color: root.color
			}
		}
	}
}
