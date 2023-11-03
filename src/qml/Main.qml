import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline
import QtQuick3D
import QtQuick3D.Helpers

import PolydotTransformationUi
import MathUtils

ApplicationWindow {
	id: window

	visible: true

	width: 1280
	height: 720

	View3D {
		id: v3d

		anchors.left: controlsPane.right
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		camera: cameraNode

		environment: SceneEnvironment {
			id: env

			antialiasingMode: SceneEnvironment.MSAA
			antialiasingQuality: SceneEnvironment.VeryHigh

			backgroundMode: SceneEnvironment.Color
			clearColor: "#002b36"
		}

		Node {
			id: originNode

			PerspectiveCamera {
				id: cameraNode
				position: Qt.vector3d(200, 200, 1000)
			}
		}

		DirectionalLight {
			id: directionalLight
			color: Qt.rgba(0.4, 0.2, 0.6, 1.0)
			ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
		}

		PointLight {
			id: pointLight
			position: Qt.vector3d(0, 0, 100)
			color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
			ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
		}

		AxisHelper {
			enableXZGrid: false
			enableXYGrid: true
			gridColor: Qt.rgba(0.8, 0.8, 0.8, 0.5)
		}

		Model {
			id: triangleModel
			visible: false
			scale: Qt.vector3d(100, 100, 100)
			geometry: ExampleTriangleGeometry {
				normals: cbNorm.checked
				normalXY: sliderNorm.value
				uv: cbUV.checked
				uvAdjust: sliderUV.value
			}
			materials: [
				DefaultMaterial {
					Texture {
						id: baseColorMap
						source: "qrc:/PolydotTransformations/src/res/logo.png"
					}
					cullMode: DefaultMaterial.NoCulling
					diffuseMap: cbTexture.checked ? baseColorMap : null
					specularAmount: 0.5
				}
			]
		}

		Node {
			id: polidot

			visible: false

			DefaultMaterial {
				id: polidotMaterial
				lighting: DefaultMaterial.NoLighting
				cullMode: DefaultMaterial.NoCulling
				diffuseColor: "yellow"
				pointSize: sliderPointSize.value * 100
				lineWidth: sliderPointSize.value * 100
			}

			PointWithName {
				id: originBasis1
				text: "Basis_1"
				pos: Qt.vector2d(slBasis1X.value * 10, slBasis1Y.value * 10)
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis1
				text: "Basis_1`"
				pos: Qt.vector2d(slResBasis1X.value * 10, slResBasis1Y.value * 10)
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: originBasis2
				text: "Basis_2"
				pos: Qt.vector2d(slBasis2X.value * 10, slBasis2Y.value * 10)
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis2
				text: "Basis_2`"
				pos: Qt.vector2d(slResBasis2X.value * 10, slResBasis2Y.value * 10)
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: originBasis3
				text: "Basis_3"
				pos: Qt.vector2d(slBasis3X.value * 10, slBasis3Y.value * 10)
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis3
				text: "Basis_3`"
				pos: Qt.vector2d(slResBasis3X.value * 10, slResBasis3Y.value * 10)
				onPosChanged: resultLine.update()
			}

			Model {
				id: originalLine
				scale: Qt.vector3d(100, 100, 100)
				geometry: LineGeometry {
					p1: Qt.vector2d(0, 2)
					p2: Qt.vector2d(5, 6)
				}
				materials: [ polidotMaterial ]
			}

			Model {
				id: resultLine

				scale: Qt.vector3d(100, 100, 100)
				geometry: LineGeometry {
					p1: Qt.vector2d(0, 0)
					p2: Qt.vector2d(0, 1)
				}
				materials: [ polidotMaterial ]

				function update() {
					const origBasises = [originBasis1.geom, originBasis2.geom, originBasis3.geom];
					const resBasises = [resultBasis1.geom, resultBasis2.geom, resultBasis3.geom];
					const resLineGeom = MathUtils.getPolydotLine(originalLine.geometry,
										origBasises, resBasises);
					if (!resLineGeom) {
						resultLine.geometry = LineGeometry;
						return;
					}
					resultLine.geometry = resLineGeom;
				}
			}

			MeshList {
				//model: controller.
				scale: controller.globalScale
			}
		}

		Model {
			id: torusModel
			visible: false
			geometry: TorusMesh {
				radius: radiusSlider.value
				tubeRadius: tubeRadiusSlider.value
				segments: segmentsSlider.value
				rings: ringsSlider.value
			}
			materials: [
				PrincipledMaterial {
					id: torusMaterial
					baseColor: "#dc322f"
					metalness: 0.0
					roughness: 0.1
				}
			]
		}

		OrbitCameraController {
			origin: originNode
			camera: cameraNode
		}
	}

	Pane {
		id: controlsPane

		ColumnLayout {
			GroupBox {
				title: "Mode"
				ButtonGroup {
					id: modeGroup
					buttons: [ radioCustGeom, radioPointGeom, radioQMLGeom ]
				}
				ColumnLayout {
					RadioButton {
						id: radioCustGeom
						text: "Triangle"
						checked: false
					}
					RadioButton {
						id: radioPointGeom
						text: "Polydot"
						checked: true
					}
					RadioButton {
						id: radioQMLGeom
						text: "Thor"
						checked: false
					}
				}
			}

			Pane {
				id: gridSettings
				visible: false
				ColumnLayout {
					Button {
						text: "+ Y Cells"
						onClicked: grid.horizontalLines += 1
						Layout.alignment: Qt.AlignHCenter

					}
					RowLayout {
						Layout.alignment: Qt.AlignHCenter
						Button  {
							text: "- X Cells"
							onClicked: grid.verticalLines -= 1
						}
						Button {
							text: "+ X Cells"
							onClicked: grid.verticalLines += 1
						}
					}
					Button  {
						text: "- Y Cells"
						onClicked: grid.horizontalLines -= 1
						Layout.alignment: Qt.AlignHCenter
					}

					Label {
						text: "Line width (if supported)"
					}
					Slider {
						Layout.fillWidth: true
						id: sliderLineWidth
						from: 1.0
						to: 10.0
						stepSize: 0.5
						value: 1.0
					}
				}
			}
			Pane {
				id: triangleSettings
				visible: false
				ColumnLayout {
					CheckBox {
						id: cbNorm
						text: "provide normals in geometry"
						checked: false
					}
					RowLayout {
						enabled: cbNorm.checked
						Label {
							Layout.fillWidth: true
							text: "Normal adjust: "
						}
						Slider {
							id: sliderNorm

							from: 0.0
							to: 1.0
							stepSize: 0.01
							value: 0.0
						}
					}
					CheckBox {
						id: cbTexture
						text: "enable base color map"
						checked: false
					}
					CheckBox {
						id: cbUV
						text: "provide UV in geometry"
						checked: false
					}
					RowLayout {
						enabled: cbUV.checked
						Label {
							Layout.fillWidth: true
							text: "UV adjust:"
						}
						Slider {
							id: sliderUV
							from: 0.0
							to: 1.0
							stepSize: 0.01
							value: 0.0
						}
					}
				}

			}
			Pane {
				id: geometrySettings

				property int sliderWidth: 100

				visible: false

				ColumnLayout {
					anchors.left: parent.left
					anchors.right: parent.right

					TextWithSlider {
						id: sliderPointSize

						visible: false

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Point size:"
						minValue: 0.01
						maxValue: 0.16
						step: 0.01
						value: 0.07
					}

					TextWithSlider {
						id: slBasis1X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis1 X:"
						value: 0.2
					}

					TextWithSlider {
						id: slBasis1Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis1 Y:"
						value: 0.2
					}

					TextWithSlider {
						id: slResBasis1X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis1` X:"
						value: 0.6
					}

					TextWithSlider {
						id: slResBasis1Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis1` Y:"
						value: 0.4
					}

					MenuSeparator {
						Layout.preferredWidth: parent.implicitWidth
						padding: 0
					}

					// Basis2

					TextWithSlider {
						id: slBasis2X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis2 X:"
						value: 0.4
					}

					TextWithSlider {
						id: slBasis2Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis2 Y:"
						value: 0.1
					}

					TextWithSlider {
						id: slResBasis2X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis2` X:"
						value: 0.7
					}

					TextWithSlider {
						id: slResBasis2Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis2` Y:"
						value: 0.3
					}

					MenuSeparator {
						Layout.preferredWidth: parent.implicitWidth
						padding: 0
					}

					// Basis3

					TextWithSlider {
						id: slBasis3X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis3 X:"
						value: 0.4
					}

					TextWithSlider {
						id: slBasis3Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis3 Y:"
						value: 0.1
					}

					TextWithSlider {
						id: slResBasis3X

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis3` X:"
						value: 0.7
					}

					TextWithSlider {
						id: slResBasis3Y

						sliderWidth: geometrySettings.sliderWidth
						labelText: "Basis3` Y:"
						value: 0.3
					}
				}
			}
			Pane {
				id: torusSettings
				visible: false
				ColumnLayout {
					Label {
						text: "Radius: (" + radiusSlider.value + ")"
					}
					Slider {
						id: radiusSlider
						from: 1.0
						to: 1000.0
						stepSize: 1.0
						value: 200
					}
					Label {
						text: "Tube Radius: (" + tubeRadiusSlider.value + ")"
					}
					Slider {
						id: tubeRadiusSlider
						from: 1.0
						to: 500.0
						stepSize: 1.0
						value: 50
					}
					Label {
						text: "Rings: (" + ringsSlider.value + ")"
					}
					Slider {
						id: ringsSlider
						from: 3
						to: 35
						stepSize: 1.0
						value: 20
					}
					Label {
						text: "Segments: (" + segmentsSlider.value + ")"
					}
					Slider {
						id: segmentsSlider
						from: 3
						to: 35
						stepSize: 1.0
						value: 20
					}
					CheckBox {
						id: wireFrameCheckbox
						text: "Wireframe Mode"
						checked: false
						onCheckedChanged: {
							env.debugSettings.wireframeEnabled = checked
							torusMaterial.cullMode = checked ? Material.NoCulling : Material.BackFaceCulling

						}
					}
				}
			}
			//DebugView {
			//	 source: v3d
			//	 resourceDetailsVisible: true
			// }
		}
		states: [
			State {
				name: "triangleMode"
				when: radioCustGeom.checked
				PropertyChanges {
					triangleModel.visible: true
					triangleSettings.visible: true
					env.debugSettings.wireframeEnabled: false
					//originNode.position: Qt.vector3d(0, 0, 0)
					originNode.rotation: Qt.quaternion(1, 0, 0, 0)
					//cameraNode.z: 600
				}
			},
			State {
				name: "polidotMode"
				when: radioPointGeom.checked
				PropertyChanges {
					polidot.visible: true
					geometrySettings.visible: true
					env.debugSettings.wireframeEnabled: false
					//originNode.position: Qt.vector3d(0, 0, 0)
					originNode.rotation: Qt.quaternion(1, 0, 0, 0)
					//cameraNode.z: 600
				}
			},
			State {
				name: "qmlMode"
				when: radioQMLGeom.checked
				PropertyChanges {
					torusModel.visible: true
					torusSettings.visible: true
					directionalLight.eulerRotation: Qt.vector3d(-40, 0, 0)
					directionalLight.color: "white"
					pointLight.color: "white"
					pointLight.position: Qt.vector3d(0, 0, 0)
					//originNode.position: Qt.vector3d(0, 0, 0)
					originNode.eulerRotation: Qt.vector3d(-20, 0, 0)
					//cameraNode.z: 600
				}
			}
		]
	}

	MainController {
		id: controller
	}

//	Timeline {
//		id: timeline
//		startFrame: 0
//		endFrame: 1000
//		currentFrame: 0
//		enabled: true
//		animations: [
//			TimelineAnimation {
//				duration: 5000
//				from: 0
//				to: 1000
//				running: true
//			}
//		]

//		KeyframeGroup {
//			target: originNode
//			property: "eulerRotation.y"

//			Keyframe {
//				frame: 0
//				value: 0
//			}
//			Keyframe {
//				frame: 250
//				value: 90
//			}
//			Keyframe {
//				frame: 750
//				value: -90
//			}
//			Keyframe {
//				frame: 1000
//				value: 0
//			}
//		}
//	}


	//	MouseArea {
	//		anchors.fill: v3d

	//		hoverEnabled: true

	//		property var lastPressPos: null

	//		onPressed: /*mouse =>*/ {
	//			if ((mouse.modifiers & Qt.ShiftModifier)
	//					&& mouse.button === Qt.LeftButton) {
	//				lastPressPos = Qt.point(mouse.x, mouse.y)
	//				console.log("(" + mouse.x + ", " + mouse.y + ")")
	//			}
	//		}

	//		onReleased: /*mouse =>*/ {
	//			if (!!lastPressPos) {
	//				lastPressPos = null;
	//			}
	//		}

	//		onPositionChanged: {
	//			if (!lastPressPos) {
	//				return;
	//			}
	//			const dx = mouse.x - lastPressPos.x;
	//			const dy = mouse.y - lastPressPos.y;
	//			cameraNode.x += dx / 100;
	//			cameraNode.y += dy / 100;
	//		}
	//	}
}
