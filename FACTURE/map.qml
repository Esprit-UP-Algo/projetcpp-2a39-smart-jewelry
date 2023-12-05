import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    id: window

    property double oldLat: 36.898237
    property double oldLng:  10.189049
    property Component comMarker: mapMarker
    property Component comMarker2: mapMarker2
    property Component comMarker3: mapMarker3
    property Component comMarker4: mapMarker4
    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldLat, oldLng);
        zoomLevel: 3

    }

    function setCenter(lat, lng) {
        mapView.pan(oldLat - lat, oldLng - lng)
        oldLat = lat
        oldLng = lng
    }

    function addMarker(lat, lng) {
        var item = comMarker.createObject(window, {
                                           coordinate: QtPositioning.coordinate(lat, lng)
                                          })
        mapView.addMapItem(item)
    }
    function addMarker2(lat, lng) {
        var item = comMarker2.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }
    function addMarker3(lat, lng) {
        var item = comMarker3.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }
    function addMarker4(lat, lng) {
        var item = comMarker4.createObject(window, {
            coordinate: QtPositioning.coordinate(lat, lng)
        })
        mapView.addMapItem(item)
    }

    Component {
        id: mapMarker
        MapQuickItem {
            id: markerImg
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
            }
        }
    }
     Component{
        id: mapMarker2
        MapQuickItem {
            id: markerImg2
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image2
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_green.png"
            }
        }
    }
     Component{
        id: mapMarker3
        MapQuickItem {
            id: markerImg3
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image4
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_green.png"
            }
        }
    }
     Component{
        id: mapMarker4
        MapQuickItem {
            id: markerImg4
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image4
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_green.png"
            }
        }
    }
}
