%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Use add_room. to create room first.
% Use add_course to add a course.
% Use add_student to add a student.
% All of them must be added with valid and compatible information. 
% So it is impossible to have a scheduling conflict.
% Use can_student_enroll(StudentID, CourseID). to see if a student with StudentID can enroll in a class with CourseID.
% Use can_room_assigned(CourseID, RoomID). to see if a course with CourseID can enroll in a room with RoomID.
% Don't use assert() rule to add room, course or student. It might give wrong results.

:- dynamic room/6.
:- dynamic course/7.
:- dynamic student/3.

%room(ID, Capacity, Equipment, HandicappedSupport, StartHour, EndHour).
%course(ID, Instructor, Capacity, Hours, Room, Needs, StudentNum).
%instructor(ID, [Courses]).
%student(ID, [Courses], Handicapped).

%Checks if the entered room hour is between 8 and 17.
room_hour_isvalid(StartHour, EndHour) :-
    StartHour < EndHour,
    8 =< StartHour,
    EndHour =< 17.

%Checks if the entered course hour is between the operation hours of the room.
course_hour_isvalid(Room, CourseHour) :-
    room(Room, _, _, _, StartHour, EndHour),
    CourseHour =< EndHour,
    StartHour =< CourseHour.

%Checks if the room is already occupied by another course at that hour.
occupied(Room, Hour) :-
    room(Room, _, _, _, _, _),
    course(_, _, _, Hour, Room, _, _).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Checks if room satisfies the equipment needs of the course.
require(Room, NeededEquipments) :-
    room(Room, _, RoomEquipments, _, _, _),
    checkRequirements(RoomEquipments, NeededEquipments).

%When NeededEquipments is empty, needs are satisfied.
checkRequirements(_, []).

%Get head of  the list and call next rule with it.
checkRequirements(RoomEquipments, NeededEquipments) :-
    [X|_] = NeededEquipments,
    checkRequirements(RoomEquipments, NeededEquipments, X).

%Checks if the equipment X is in the room and calls checkRequirements rule for the remaining of the list.
checkRequirements(RoomEquipments, NeededEquipments, X) :-
    member(X, RoomEquipments),
    [X|Tail] = NeededEquipments,
    checkRequirements(RoomEquipments, Tail).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Checks if the room has enough capacity for the course.
enoughCapacity(Room, Capacity) :-
    room(Room, RoomCapacity, _, _, _, _),
    Capacity < RoomCapacity.

%Checks if the instructor is busy at that hour.
instructor_busy(Instructor, Hour) :-
    course(_, Instructor, _, Hour, _, _, _).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%If list is emptied, it is true.
check_courseSize([]).

%Get head of the list and call check_courseSize rule.
check_courseSize(Courses) :-
    [X|_] = Courses,
    check_courseSize(X, Courses).

%Checks if the course X has filled its capacity.
check_courseSize(X, Courses) :-
    member(X, Courses),
    course(X, _, Capacity, _, _, _, StudentNum),
    StudentNum < Capacity,
    [X|Tail] = Courses,
    check_courseSize(Tail).
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Checks if the room of a course has handicapped student support.
check_handicap(Courses, Handicapped) :-
    Handicapped = 'yes',
    member(X, Courses),
    course(X, _, _, _, Room, _,_),
    room(Room, _, _, Handicapped, _, _).

check_handicap(_, Handicapped) :-
    Handicapped = 'no'.
    

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%This is to read user input as a list until whatever End is. I found it from the internet.
read_list_until(L, End) :-
    (   read_element(E, End)
    ->  L = [E|L1],
        read_list_until(L1, End)
    ;   L = []
    ).

read_element(E, End) :-
    read(E),
    dif(E, End).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%This part is to increment all the courses' sizes when a new student gets them. 
%If Courses is empty, job is done. 
set_course_size([]).

%Get the head of the list and call set_course_size/2 rule.
set_course_size(Courses) :- 
    [X|_] = Courses,
    set_course_size(X, Courses).

%Increment size of course X and call set_course_size for the remaining of the list.
set_course_size(X, Courses) :-
    member(X, Courses),
    retract(course(X, Instructor, Capacity, Hours, Room, Needs, Size)),
    NewSize is Size + 1,
    assert(course(X, Instructor, Capacity, Hours, Room, Needs, NewSize)),
    [X|Tail] = Courses,
    set_course_size(Tail).
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%This part is to compare the time of the course Y and a list of Courses.
compare_courseTimes([], _).

%Get head of the list and call compare_courseTimes/3 rule. 
compare_courseTimes(Courses, Y) :-
    [X|_] = Courses,
    compare_courseTimes(X, Courses, Y).

%Checks if times of two courses are equal or not. Then call compare_courseTimes/2 rule for the remaining of the list.
compare_courseTimes(X, Courses, Y) :-
    course(X, _, _, XHour, _, _, _),
    course(Y, _, _, YHour, _, _, _),
    XHour \= YHour,
    [X|Tail] = Courses,
    compare_courseTimes(Tail, Y).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%This part checks if a student can enroll to a course CourseID. 
%If student is already assigned to the course, he/she can take it.
can_student_enroll(StudentID, CourseID) :-
    student(StudentID, Courses, _),
    member(CourseID, Courses).
%It calls previousy defined rules to check enrollment ability.
can_student_enroll(StudentID, CourseID) :-
    student(StudentID, Courses, Handicapped),
    course(CourseID, _, _, _, _, _, _),
    check_courseSize(Courses),
    check_handicap(Courses, Handicapped),
    compare_courseTimes(Courses, CourseID).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%This part checks if a room can be assigned to a course CourseID.
%If course is already assigned to this room, it can be assigned.
can_room_assigned(CourseID, RoomID) :-
    course(CourseID, _, _, _, RoomID, _, _).

%Calls previously defined rules to check assigning ability. 
can_room_assigned(CourseID, RoomID) :-
    room(RoomID, _, _, _, _, _),
    course(CourseID, _, CourseCapacity, CourseHour, _, Needs, _),
    \+ occupied(RoomID, CourseHour),
    course_hour_isvalid(RoomID, CourseHour),
    require(RoomID, Needs),
    enoughCapacity(RoomID, CourseCapacity).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Next parts are for adding new room, course and students to the system.
%Required informations are obtained from the the user, validity checks are done by calling some rules,
%and if there isn't any conflict, new object is added to the system.

add_room :-
    write('Enter room ID: '),
    read(ID),
    write('Enter room capacity: '),
    read(Capacity),
    write('What are the operations hours of this room? Enter two hours: '),
    read(StartHour),
    read(EndHour),
    write('Does this room have special equipment? Enter their names one by one. Enter \'stop\' to finish or there isn\'t any equipment: '),
    read_list_until(Equipments, stop),
    write('Does this room have support for handicapped students? Enter \'yes\' or \'no\': '),
    read(HandicappedSupport),
    room_hour_isvalid(StartHour, EndHour),
    assert(room(ID, Capacity, Equipments, HandicappedSupport, StartHour, EndHour)).

add_course :-
    write('Enter course ID: '),
    read(ID),
    write('Enter course instructor: '),
    read(Instructor),
    write('Enter course capacity: '),
    read(Capacity),
    write('Enter course hour: '),
    read(Hour),
    write('Enter course room: '),
    read(Room),
    write('Does this course have special needs? Enter their names one by one. Enter \'stop\' to finish or there isn\'t any equipment: '),
    read_list_until(Needs, stop),
    \+ occupied(Room, Hour),
    course_hour_isvalid(Room, Hour),
    require(Room, Needs),
    enoughCapacity(Room, Capacity),
    \+ instructor_busy(Instructor, Hour),
    assert(course(ID, Instructor, Capacity, Hour, Room, Needs, 0)).

add_student :-
    write('Enter sudent ID: '),
    read(ID),
    write('Enter courses of the student one by one: Enter \'stop\' to finish.'),
    read_list_until(Courses, stop),
    write('Is the student handicapped? Enter \'yes\' or \'no\':'),
    read(Handicapped),
    check_courseSize(Courses),
    check_handicap(Courses, Handicapped),
    assert(student(ID, Courses, Handicapped)),
    set_course_size(Courses).