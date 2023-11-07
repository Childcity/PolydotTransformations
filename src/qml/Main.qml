import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline
import QtQuick3D
import QtQuick3D.Helpers

import PolydotTransformationUi
import MathUtils

ApplicationWindow {
	id: root

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
				lineWidth: 7
			}

			PointWithName {
				id: originBasis1
				text: "Basis_1"
				pos: basis1Sett.slOrgVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis1
				text: "Basis_1`"
				pos: basis1Sett.slResVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: originBasis2
				text: "Basis_2"
				pos: basis2Sett.slOrgVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis2
				text: "Basis_2`"
				pos: basis2Sett.slResVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: originBasis3
				text: "Basis_3"
				pos: basis3Sett.slOrgVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis3
				text: "Basis_3`"
				pos: basis3Sett.slResVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: originBasis4
				text: "Basis_4"
				pos: basis4Sett.slOrgVector2d
				onPosChanged: resultLine.update()
			}

			PointWithName {
				id: resultBasis4
				text: "Basis_4`"
				pos: basis4Sett.slResVector2d
				onPosChanged: resultLine.update()
			}

			Model {
				id: originalLine
				visible: false
				scale: Qt.vector3d(100, 100, 100)
				geometry: LineGeometry {
					p1: Qt.vector2d(0, 2)
					p2: Qt.vector2d(5, 6)
				}
				materials: [ polidotMaterial ]
			}

			Model {
				id: resultLine
				visible: false
				scale: Qt.vector3d(100, 100, 100)
				geometry: LineGeometry {
					p1: Qt.vector2d(0, 0)
					p2: Qt.vector2d(0, 1)
				}
				materials: [ polidotMaterial ]

				function update() {
					const origBasises = [originBasis1.geom, originBasis2.geom, originBasis3.geom, originBasis4.geom];
					const resBasises = [resultBasis1.geom, resultBasis2.geom, resultBasis3.geom, resultBasis4.geom];

					controller.applyPolydotTransformations(origBasises, resBasises);

					const resLineGeom = MathUtils.getPolydotTransformedLine(originalLine.geometry,
										origBasises, resBasises);
					if (!resLineGeom) {
						resultLine.geometry = LineGeometry;
						return;
					}
					resultLine.geometry = resLineGeom;
				}
			}

			MeshList {
				model: controller.meshListModel
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

				visible: false

				component BasisSettings: ColumnLayout {
					id: bRoot

					property int sliderWidth: 100

					property alias useSlRes: swType.checked

					property int index: 1
					property string slOrgText: "Basis" + bRoot.index
					property string slResText: "Basis" + bRoot.index + swType.text

					readonly property vector2d slOrgVector2d: Qt.vector2d(slX.value * 10, slY.value * 10)
					readonly property vector2d slResVector2d: Qt.vector2d(sl1X.value * 10, sl1Y.value * 10)

					RowLayout {
						Switch {
							id: swType

							Layout.alignment: Qt.AlignHCenter

							checked: true
							text: "`"
						}

						StackLayout {
							currentIndex: bRoot.useSlRes

							ColumnLayout {
								TextWithSlider {
									id: slX

									sliderWidth: bRoot.sliderWidth
									labelText: bRoot.slOrgText + " X:"
									value: 0.2
								}
								TextWithSlider {
									id: slY

									sliderWidth: bRoot.sliderWidth
									labelText: bRoot.slOrgText + " Y:"
									value: 0.2
								}
							}
							Column {
								TextWithSlider {
									id: sl1X

									sliderWidth: bRoot.sliderWidth
									labelText: bRoot.slResText + " X:"
									value: 0.2
								}
								TextWithSlider {
									id: sl1Y

									sliderWidth: bRoot.sliderWidth
									labelText: bRoot.slResText + " Y:"
									value: 0.2
								}
							}
						}
					}

					MenuSeparator {
						Layout.preferredWidth: sl1X.implicitWidth + swType.implicitWidth
					}
				}

				ColumnLayout {
					anchors.left: parent.left
					anchors.right: parent.right

					BasisSettings {
						id: basis1Sett
						index: 1
					}

					BasisSettings {
						id: basis2Sett
						index: 2
					}
					BasisSettings {
						id: basis3Sett
						index: 3
					}
					BasisSettings {
						id: basis4Sett
						index: 4
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

	Connections {
        target: Application

        function onStateChanged() {
			if (Application.state === Qt.ApplicationActive) {
				controller.loadMeshes();
			}
        }
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
