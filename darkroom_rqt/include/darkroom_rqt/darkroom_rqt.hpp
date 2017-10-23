#pragma once

#ifndef Q_MOC_RUN

#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <darkroom_rqt/ui_darkroom_rqt.h>
#include <QWidget>
#include <pluginlib/class_list_macros.h>
#include <QStringList>
#include <darkroom/LighthouseSimulator.hpp>
#include <darkroom/TrackedObject.hpp>
#include <roboy_communication_middleware/LighthousePoseCorrection.h>
#include <map>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>

#endif

class RoboyDarkRoom
        : public rqt_gui_cpp::Plugin {
    Q_OBJECT
public:
    RoboyDarkRoom();
    ~RoboyDarkRoom();

    virtual void initPlugin(qt_gui_cpp::PluginContext &context);

    virtual void shutdownPlugin();

    virtual void saveSettings(qt_gui_cpp::Settings &plugin_settings,
                              qt_gui_cpp::Settings &instance_settings) const;

    virtual void restoreSettings(const qt_gui_cpp::Settings &plugin_settings,
                                 const qt_gui_cpp::Settings &instance_settings);

public Q_SLOTS:
    /**
     * connect Roboy via ROS
     */
    void connectRoboy();

    /**
     * connect simulated Roboy via ROS
     */
    void simulateRoboy();

    /**
     * connect Object via UDP socket
     */
    void connectObject();

    /**
     * Clears all visualizations in rviz
     */
    void clearAll();
    /**
         * Resets the lighthouse poses to slider values
         */
    void resetLighthousePoses();
    /**
         * Resets the object poses to slider values
         */
    void resetObjectPoses();

    /**
     * Toggles recording sensor values for all tracked objects
     */
    void record();

    /**
     * Toggles visualization of lighthouse rays
     */
    void showRays();

    /**
     * Toggles visualization of relative sensor distances
     */
    void showDistances();


    void switchLighthouses();

    /**
     * Toggles tracking thread
     * @param start toggle flag
     */
    void startTriangulation();

    /**
     * Toggles calibration thread
     * @param start bool
     */
    void startCalibrateRelativeSensorDistances();

    /**
     * Toggles poseestimation thread
     * @param start bool
     */
    void startPoseEstimationSensorCloud();

    /**
     * Toggles distance estimation thread
     * @param start bool
     */
    void startEstimateSensorPositionsUsingRelativeDistances();

    /**
     * Toggles particle filter thread for pose correction
     * @param start bool
     */
    void startPoseEstimationParticleFilter();
private:
    /**
         * Is regularily publishing the tf frames (lighthouse1, lighthouse2)
         */
    void transformPublisher();

    /**
     * Callback for pose correction message
     * @param msg
     */
    void correctPose(const roboy_communication_middleware::LighthousePoseCorrection &msg);
private:
    Ui::RoboyDarkRoom ui;
    QWidget *widget_;

    QVector<double> time;
    int counter = 0;
    QColor color_pallette[14] = {Qt::blue, Qt::red, Qt::green, Qt::cyan, Qt::magenta, Qt::darkGray, Qt::darkRed, Qt::darkGreen,
                                 Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow, Qt::black, Qt::gray};
    ros::NodeHandlePtr nh;
    boost::shared_ptr<ros::AsyncSpinner> spinner;
    boost::shared_ptr<std::thread> transform_thread = nullptr;
    ros::Subscriber pose_correction_sub;
    tf::TransformListener tf_listener;
    tf::TransformBroadcaster tf_broadcaster;
    static tf::Transform lighthouse1, lighthouse2, tf_world, simulated_object_lighthouse1, simulated_object_lighthouse2;
    atomic<bool> publish_transform;
    int object_counter = 0;
    map<int, TrackedObjectPtr> trackedObjects;
    mutex mux;
    static map<string, QLineEdit*> text;
    static map<string, QSlider*> slider;
    static map<string, QPushButton*> button;
    bool simulate = false;

    map<int, boost::shared_ptr<LighthouseSimulator>> lighthouse_simulation;
};