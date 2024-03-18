// contracts/MyToken.sol
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";

contract GradCertificate is ERC20 {
    address public university;
    mapping(address => bool) public passedCourses;
    mapping(address => bool) public hasGraduated;

    constructor() ERC20("Certificate", "CRF") {
        university = msg.sender;
    }

    modifier onlyUniversity() {
        require(msg.sender == university, "Only the university can call this function");
        _;
    }

    function computeFinalGrade(address studentAddress, bool finalGrade) public onlyUniversity {
        passedCourses[studentAddress] = finalGrade;
    }

    function hasPassedCourse(address studentAddress) public view returns (bool) {
        require(!passedCourses[studentAddress], "You have passed the course already!");
        return passedCourses[studentAddress];
    }

    function hasGraduatedStudent(address studentAddress) public view returns (bool) {
        require(!hasGraduated[studentAddress], "Congratulations, you have already graduated!");
        return hasGraduated[studentAddress];
    }

    function markPassCourse(address studentAddress) public onlyUniversity {
        if (!hasPassedCourse(studentAddress)) {
            computeFinalGrade(studentAddress,true);
        }
    }

    function markGraduate(address studentAddress) public onlyUniversity {
        if (!hasGraduatedStudent(studentAddress)){
            require(passedCourses[studentAddress], "Please pass the course first");
            hasGraduated[studentAddress] = true;
        }
    }
}