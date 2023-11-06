import QtQuick
import QtQuick3D
import QtQml


Node {
	id: root

	property alias model: mesh.model

	Repeater3D {
		id: mesh

		delegate: Loader3D {
			required property int index
			required property QtObject lineGeometry

			asynchronous: true

			Model {
				visible: false
				geometry: lineGeometry
				materials: [
					DefaultMaterial {
						lighting: DefaultMaterial.NoLighting
						cullMode: DefaultMaterial.NoCulling
						diffuseColor: "lightgreen"
						lineWidth: 3
					}
				]

				Timer {
				    id: timer
				    interval: 0*index * 300
				    repeat: false
				    running: true
				    triggeredOnStart: false
				    onTriggered:
				    {
				        parent.visible=true
				    }
				}
			}
		}
	}
}
