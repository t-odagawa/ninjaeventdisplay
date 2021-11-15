#cd ${EVENT_DISPLAYDIR}/bin
cd ${HOME}/WagasciEventDisplay/build/src

inputdir=${HOME}/data/hitconv
inputfile=${inputdir}/track_b2physics_wagasci_babymind_2021_ninja_nodead_$1_$2_$3.root

#outputdir=${HOME}/SharingFile/display/output
outputdir=${HOME}/data/plots

geometrydir=${HOME}/Programs/WagasciMC/etc/wagasci/b2/geometry

entry=$4

eventid=$5

command=${HOME}/SharingFile/display/input/command.txt

./EventDisplay --input-file ${inputfile} --output-directory ${outputdir} --geometry-directory ${geometrydir} --start-event ${entry} -l pdf < ${command}
mv ${outputdir}/event_display_${entry}.pdf ${outputdir}/event_display_${eventid}.pdf
