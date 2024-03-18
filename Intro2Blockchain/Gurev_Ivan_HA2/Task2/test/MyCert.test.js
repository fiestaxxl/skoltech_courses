const { expect } = require("chai");

const {
    loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");



describe("Certificate", function () {
    async function deployTokenFixture() {
        const [university, addr1, addr2] = await ethers.getSigners();
        const graduationCertificates = await ethers.deployContract("GradCertificate");

        await graduationCertificates.waitForDeployment();
        return { graduationCertificates, university, addr1, addr2 };
    }

    describe("Deployment", function () {
        it("Test scenario 1. Deploying contract", async function () {
            const { graduationCertificates, university } = await loadFixture(deployTokenFixture);

            expect(await graduationCertificates.university()).to.equal(university.address);
        });
    });

    describe("Functionality", function () {
        it("Test case 1: Only the university can issue a certificate of graduation", async function () {
            const { graduationCertificates, university, addr1 } = await loadFixture(deployTokenFixture);
            await expect(graduationCertificates.connect(addr1).computeFinalGrade(addr1.address, true)).to.be.revertedWith("Only the university can call this function");
        });

        it("Test case 2: The student cannot be issued a certificate if he didn’t pass all courses", async function () {
            const { graduationCertificates, university, addr1 } = await loadFixture(deployTokenFixture);
            await expect(graduationCertificates.connect(university).markGraduate(addr1.address)).to.be.revertedWith("Please pass the course first");
        });

        it("Test case 3: Only the university can pass all the student", async function () {
            const { graduationCertificates, university, addr1 } = await loadFixture(deployTokenFixture);
            await graduationCertificates.connect(university).computeFinalGrade(addr1.address, true);
            await expect(graduationCertificates.connect(addr1).markPassCourse(addr1.address)).to.be.revertedWith("Only the university can call this function");
          });

          it("Test case 4: Once a student is issued a certificate, he cannot be issued one again", async function () {
            const { graduationCertificates, university, addr1 } = await loadFixture(deployTokenFixture);
            await graduationCertificates.connect(university).computeFinalGrade(addr1.address, true);
            await graduationCertificates.connect(university).markGraduate(addr1.address);
            await expect(graduationCertificates.connect(university).markGraduate(addr1.address)).to.be.revertedWith("Congratulations, you have already graduated!");
          });
    });
});