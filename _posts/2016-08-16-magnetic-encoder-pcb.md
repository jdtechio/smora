---
title: "Magnetic encoder PCB"
date: 2016-08-18T17:00:00+01:00
categories:
  - Hardware
tags:
  - PCB
  - Hardware
  - MG996R
  - AS5048B
  - MPU-6050
  - DS18B20
  - Fusion 360
  - Eagle
gallery:
  - url: mag_encoder_pcb_pair_a_bottom.jpg
    image_path: mag_encoder_pcb_pair_a_bottom_tn.jpg
    alt: "PCB bottom with magnet assembly"
    title: "PCB bottom with magnet assembly"
  - url: mag_encoder_pcb_pair_b_bottom.jpg
    image_path: mag_encoder_pcb_pair_b_bottom_tn.jpg
    alt: "PCB bottom without magnet assembly"
    title: "PCB bottom without magnet assembly"
gallery2:
  - url: mag_encoder_pcb_pair_b_top.jpg
    image_path: mag_encoder_pcb_pair_b_top_tn.jpg
    alt: "PCB top size comparison"
    title: "PCB top size comparison"
gallery3:
  - url: mag_encoder_AS5048B_I2C.jpg
    image_path: mag_encoder_AS5048B_I2C.jpg
    alt: "I2C communication with AS5048B sensor"
    title: "I2C communication with AS5048B sensor"
  - url: mag_encoder_AS5048B_I2C_possible_crosstalk.jpg
    image_path: mag_encoder_AS5048B_I2C_possible_crosstalk.jpg
    alt: "I2C communication with AS5048B sensor zoomed in"
    title: "I2C communication with AS5048B sensor zoomed in"
gallery4:
  - url: mag_encoder_pcb_installed.jpg
    image_path: mag_encoder_pcb_installed_tn.jpg
    alt: "Magnetic encoder PCB installed inside the MG996R enclosure"
    title: "Magnetic encoder PCB installed inside the MG996R enclosure"
---

The prototype for the magnetic encoder board is alive and kicking!

<!--more-->

{% include base_path %}
{% capture model_rendering %}
[![ModelRendering]({{ base_path }}/images/model_rendering_low_res.png)]({{ base_path }}/images/model_rendering_high_res.png)
{% endcapture %}

{% capture test_jig %}
[![TestJig]({{ base_path }}/images/mag_encoder_test_rig_tn.jpg)]({{ base_path }}/images/mag_encoder_test_rig.jpg)
{% endcapture %}

{% capture main_gear %}
[![TestJig]({{ base_path }}/images/MG996R_gears_tn.jpg)]({{ base_path }}/images/MG996R_gears.jpg)
{% endcapture %}

{% capture main_gear %}
[![MainGear]({{ base_path }}/images/MG996R_gears_tn.jpg)]({{ base_path }}/images/MG996R_gears.jpg)
{% endcapture %}

# Why

The **[SMORA]({{ site.github_code }})** project aims to create a smart servo motor that will offload most of the processing power and communication to the servo itself. Multiple sensors will allow for real-time feedback about the shaft position and velocity as well as the servo attitude (yaw, pitch and roll). The data available might prove interesting by, for example, giving information about individual limb orientation in a robotic arm.

Daisy-chaining several servo's through RS-485 will remove the tangled mess of cables and provide a single bus for network communication.

Using a magnetic position encoder and removing the limit pin on the main shaft gear allows for a 360&deg; rotation instead of the usual constrain of 0&deg; to 180&deg;.

<figure>
  {{ main_gear | markdownify | remove: "<p>" | remove: "</p>" }}
  <figcaption>MG996R gear system. Notice the hole in the main shaft gear left from removing the limit pin.</figcaption>
</figure>

# Model

It all started with reverse engineering the TowerPro MG996R inner dimensions with a caliper. A simplified model was built using Autodesk Fusion 360. This step was necessary to get a good understanding of the available space inside the servo enclosure so that all the PCB's would fit properly. Along with the body, the holding enclosure for the diametrically magnetized neodymium magnet, shaft and an alignment guide were also designed.

<figure>
  {{ model_rendering | markdownify | remove: "<p>" | remove: "</p>" }}
  <figcaption>Model rendering depicting MG996R simplified body in grey, magnet in silver and holding enclosure in black. Shaft is hidden underneath the magnet.</figcaption>
</figure>

The shaft is directly bonded to the magnet using a cyanoacrylate glue. The alignment guide was built specifically to reduce the chance of glueing the 2 parts out of axis. Any "wobble" while the shaft is rotating would translate into a lower precision reading from the AS5048B rotary position sensor. The holding enclosure also assures that the magnet keeps centered right over the sensor's sweet spot.

According to the datasheet, any displacement radius under 0.25mm while using smaller diameter magnets is acceptable. So, mechanically, this models should be precise enough. Unfortunately, the minimum distance between the IC package surface and the magnet wasn't taken into account. Further testing is needed to see if this causes any non-linearities with the position output. If so, a new holding enclosure needs to be modeled considering the recommended distance ranging from 0.5mm to 2.5mm.

# Circuit

The [PCB]({{ site.github_code }}/tree/master/circuits/encoder_pcb/) shape was also designed using Fusion 360 and later exported to DXF. This way, when importing to Eagle, we'll know the available space to place components and tracks and also ensure that everything fits when it's fabricated. Special care was taken with the outer dimensions assuming that the board final revision would be fabricated by OSHPark. They have a minimum width of 2mm for the milling tool to route the outer board.

At this point, the AS5048B rotary position sensor, MPU-6050 accelerometer and gyro, DS18B20 1-wire temperature sensor, FPC connector and passive components were placed in the schematic and routed.

After exporting the GCode using PCBGcode and hand tweaking the resulting files, the board was milled on a modified Proxxon MF70 with CNC control running GRBL, test fitted and the components were soldered and tested. Special care was taken to avoid vias under the IC's. *Note to self: learn the skills necessary to create plated through-hole vias.*

The I2C addresses of the AS5048B and MPU-6050 were hardcoded to 0x40 and 0x68. All the components on this board operate at 3.3V. Pull-up resistors for the SDA and SCL lines were omitted in this revision but added later on to the Eagle files.

The FPC connector includes an interrupt pin for the MPU-6050, SCL, SDA, VCC, 1-Wire for the DS18B20, GND and 2 test pads not connected to anything at the moment.

{% include gallery id="gallery" class="full" caption="PCB bottom view with and without magnet assembly. Notice the 3D printed holding enclosure and shaft glued to the magnet on the picture to the right." %}

{% include gallery id="gallery2" class="full" caption="PCB top view size comparison." %}

{% include gallery id="gallery4" class="full" caption="PCB installed inside the MG996R enclosure. The cutouts were made so that the inside could be measured with a caliper." %}

# Test Jig

An [FPC cable breakout]({{ site.github_code }}/tree/master/circuits/encoder_cable_breakout/) was designed to make this breadboard-compatible. While at it, a common anode RGB led was added for debugging.

Because the whole board runs at 3.3V, there was a need to have a development board running at the same voltage or use some sort of voltage translator interface based, for example, on the BSS138 Mosfet. To make it simple, a 5V 16MHz Arduino Pro Micro clone was fitted with a 3.3V regulator instead of the original 5V. This of course would mean that the oscillator would also have to be replaced. However, even though it's in fact overclocked and out of spec, the board seemed to be happy to run at 3.3V 16MHz. Further testing is needed to make sure it does actually run properly. This approach is obviously not recommended.

<figure>
  {{ test_jig | markdownify | remove: "<p>" | remove: "</p>" }}
  <figcaption>Test jig with an Arduino Pro Micro clone connected to the encoder board through a custom FPC breakout board. Notice the breadboarded pull-up resistors next to the Arduino.</figcaption>
</figure>

# Software

Example [sketches]({{ site.github_code }}/tree/master/sketches/) from libraries available online were uploaded to the development board to test the individual components. At first glance, all of them passed.

Below are a couple of shots from the I2C communication with the AS5048B sensor. A *START* condition can be seen in the beginning as well as a possible crosstalk artifact in the SCL line. The I2C protocol should be rugged enough to handle it though. This artifact's origin might come from the fact that SDA and SCL run in parallel right next to each other. If later on it ends up being a problem then the spacing between them in the FPC flat cable should be increased by, for example, placing the GND line between them.

{% include gallery id="gallery3" class="full" caption="I2C communication with AS5048B sensor. Channel 1 is <span style=\"color: Gold\">SDA</span> and Channel 2 is <span style=\"color: DodgerBlue\">SCL</span>" %}

The video below shows the [AS5048B]({{ site.github_code }}/tree/master/sketches/AS5048B_RGB_LED/) test. The angle returned from the sensor is used to generate a RGB color by passing it to a HSI to RGB function as hue. The angle can be seen in the serial console at the back.

<iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/Awj-ps5e77U" frameborder="0" allowfullscreen></iframe><br>

The second video shows the servo attitude by sending the accelerometer and gyro data returned from the [MPU-6050]({{ site.github_code }}/tree/master/sketches/MPU6050_DMP6_PLANE/) to a processing sketch.

 <iframe width="560" height="315" src="https://www.youtube-nocookie.com/embed/QTiknWq8EFs" frameborder="0" allowfullscreen></iframe><br>

All the files can be found at the [SMORA GitHub Repo]({{ site.github_code }}).
