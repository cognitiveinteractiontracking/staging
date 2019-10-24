# com_viz package

This is a depricated package from robocup which showes the rsb communication in rviz between two robots.
Maybe it is worthit to port it to ROS?

## Usage examples

  <!-- visualize RSB communication -->
  <node pkg="com_viz" type="com_viz" name="com_viz_state" respawn="true" >
    <param name="rsb_viz_scope" value="/" /> <!--Viz everything on the root scope-->
    <param name="rsb_viz_subscope" value="" /> <!--Don't care about the subscope-->
    <param name="rsb_viz_content" value="" /> <!--Don't care about the content-->
    <param name="rsb_viz_destroy_content" value="" /> <!--We don't want to have a specific message content, which destroys the marker-->
    <param name="marker_remain_time" value="3.0" /> <!--Marker is deleted after 3 second-->
    <param name="marker_scale_x" value="0.03" />
    <param name="marker_scale_y" value="0.06" />
    <param name="marker_scale_z" value="1.0" />
    <param name="marker_color_a" value="1.0" />
    <param name="marker_color_r" value="0.0" />
    <param name="marker_color_g" value="0.74" />
    <param name="marker_color_b" value="1.0" />
    <param name="marker_class" value="0" /> <!--Arrow-->
  </node>
  
  <!-- visualize waypoint notification -->
  <node pkg="com_viz" type="com_viz" name="com_viz_waypoint" respawn="true" >
    <param name="rsb_viz_scope" value="/" /> <!--Viz everything on the root scope-->
    <param name="rsb_viz_subscope" value="waypoint" /> <!--Only viz the waypoints-->
    <param name="rsb_viz_content" value="entered" /> <!--The content of the msg must be "entered", to create the marker-->
    <param name="rsb_viz_destroy_content" value="left" /> <!--The content of the msg must be "left", to destroy the marker-->
    <param name="marker_remain_time" value="-1.0" /> <!--Disable marker destruction after a time by setting a negative value-->
    <param name="marker_scale_x" value="2.0" />
    <param name="marker_scale_y" value="2.0" />
    <param name="marker_scale_z" value="0.05" />
    <param name="marker_color_a" value="0.5" />
    <param name="marker_color_r" value="1.0" />
    <param name="marker_color_g" value="0.0" />
    <param name="marker_color_b" value="0.0" />
    <param name="marker_class" value="2" /> <!--Spheer-->
  </node>
  
    <!-- visualize RSB communication -->
  <node pkg="com_viz" type="com_viz" name="com_viz_state" respawn="true" >
    <param name="rsb_viz_scope" value="/" /> <!--Viz everything on the root scope-->
    <param name="rsb_viz_subscope" value="" /> <!--Don't care about the subscope-->
    <param name="rsb_viz_content" value="" /> <!--Don't care about the content-->
    <param name="rsb_viz_destroy_content" value="" /> <!--We don't want to have a specific message content, which destroys the marker-->
    <param name="marker_remain_time" value="1.0" /> <!--Marker is deleted after 1 second-->
    <param name="marker_scale_x" value="0.03" />
    <param name="marker_scale_y" value="0.06" />
    <param name="marker_scale_z" value="1.0" />
    <param name="marker_color_a" value="1.0" />
    <param name="marker_color_r" value="0.0" />
    <param name="marker_color_g" value="1.0" />
    <param name="marker_color_b" value="0.0" />
    <param name="marker_class" value="0" /> <!--Arrow-->
  </node>
  
  <!-- visualize waypoint notification -->
  <node pkg="com_viz" type="com_viz" name="com_viz_waypoint" respawn="true" >
    <param name="rsb_viz_scope" value="/" /> <!--Viz everything on the root scope-->
    <param name="rsb_viz_subscope" value="waypoint" /> <!--Only viz the waypoints-->
    <param name="rsb_viz_content" value="entered" /> <!--The content of the msg must be "entered", to create the marker-->
    <param name="rsb_viz_destroy_content" value="left" /> <!--The content of the msg must be "left", to destroy the marker-->
    <param name="marker_remain_time" value="-1.0" /> <!--Disable marker destruction after a time by setting a negative value-->
    <param name="marker_scale_x" value="5.0" />
    <param name="marker_scale_y" value="5.0" />
    <param name="marker_scale_z" value="0.05" />
    <param name="marker_color_a" value="1.0" />
    <param name="marker_color_r" value="1.0" />
    <param name="marker_color_g" value="0.0" />
    <param name="marker_color_b" value="0.0" />
    <param name="marker_class" value="2" /> <!--Spheer-->
  </node>