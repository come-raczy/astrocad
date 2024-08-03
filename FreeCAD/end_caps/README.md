# End Caps

## Parametrized end cap

This is a metric end cap with a female thread. The parameters are:

- Diameter in mm (e.g. 42 mm for M42)
- Thickness in mm (e.g. 1.5 mm for a part 1.5 mm thick)
- Tolerance in mm (e.g. 0.4 mm for generic 3D printing)
- Pitch in mm (e.g. 0.75 mm for M42x0.75)
- Length in mm (e.g. 5 mm for a thread of 5 mm)

Note that 0.2 mm for tolerance almost works but leads to very stiff screwing.

## M90 Collimation Cap

It is just an end cap with an M90x1 thread that can be screwed in place of the focuser on a GSO RC8.
The collimation cap has a pinhole (parametrized to 2.5 mi). It is recommended to print it in white to save the effort of
painting the inside in white later.

Note that with a pinhole of 1.5 mm, with the inaccuracies of 3D printing, there is no actual hole, only a faint mark
where the hole should be.

This is used to make sure that the axis of the secondary goes through the center of the primary. This is achieved when
the reflection of the pinhole is exactly in the center of the secondary (assuming there is a marker correctly placed in
the center of the primary).

## M48 Collimation cap

An M48x0.75 collimation cap to be used behind the focuser.

This is used to ensure that the axis of the focuser is the same as the axix of the secondary. This is achieved by first
ensuring that the axis of the secondary passes through the center of the primary, and by ensuring that the reflection of
the pinhole is still in the center of the secondary, when the pinhole is in the middle of the extended focuser.

Note that the RC8 doesn't have any adjustment for this. In order to be able to correct the axix of the focuser, a
tilting adapter is required (e.g. TS-Optics M90 Tilting Adapter Flange for astrophotography). Even without the adapter,
it is useful to know if there is a tilt in the axis of the focuser, as this is an important factor to decide to
purchase a tilting adapter. A few millimeters in the focus plane is not very significant. More than 15% of the sensor
diagonal might cause vignetting and distortion in a corner.
