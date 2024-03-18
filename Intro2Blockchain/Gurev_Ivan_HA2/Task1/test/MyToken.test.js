const { expect } = require("chai");

const {
    loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");



describe("MyToken", function () {
    async function deployTokenFixture() {
        const [owner, addr1, addr2] = await ethers.getSigners();
        const hardhatToken = await ethers.deployContract("MyToken");

        await hardhatToken.waitForDeployment();
        return { hardhatToken, owner, addr1, addr2 };
    }

    describe("Deployment", function () {
        it("Test scenario 0. Deploying contract", async function () {
            const { hardhatToken, owner } = await loadFixture(deployTokenFixture);

            expect(await hardhatToken.owner()).to.equal(owner.address);
        });

        it("Test scenario 1. Total supply of tokens to the owner", async function () {
            const { hardhatToken, owner } = await loadFixture(deployTokenFixture);
            const ownerBalance = await hardhatToken.balanceOf(owner.address);
            expect(await hardhatToken.totalSupply()).to.equal(ownerBalance);
        });
    });

    describe("Transactions", function () {
        it("Test scenario 1: Send tokens to Receiver_1 and check balances", async function () {
            const { hardhatToken, owner, addr1 } = await loadFixture(
                deployTokenFixture
            );
            // Transfer 20 tokens from owner to addr1
            await expect(
                hardhatToken.transfer(addr1.address, 20)
            ).to.changeTokenBalances(hardhatToken, [owner, addr1], [-20, 20]);
        });

        it("Test scenario 2: Send tokens from Receiver_1 to Receiver_2 and check balances", async function () {
            const { hardhatToken, owner, addr1, addr2 } = await loadFixture(
                deployTokenFixture
            );

            //Transfer 50 tokens from owner to addr1
            hardhatToken.transfer(addr1.address, 50)
            // Transfer 50 tokens from addr1 to addr2
            await expect(
                hardhatToken.connect(addr1).transfer(addr2.address, 50)
            ).to.changeTokenBalances(hardhatToken, [addr1, addr2], [-50, 50]);
        });
    });

    describe("Gas and transactions check", function () {
        it("Test scenario 1. Compute gas transactions", async function () {
            const { hardhatToken, owner, addr1, addr2 } = await loadFixture(
                deployTokenFixture
            );

            const tx1 = await hardhatToken.transfer(addr1.address, 50);
            const receipt1 = await tx1.wait();
            const gas1 = receipt1.gasUsed ;
            console.log("\tgas per transaction from owner to addr1:", gas1);

            const tx = await hardhatToken.connect(addr1).transfer(addr2.address, 50);
            const receipt = await tx.wait();
            const gas = receipt.gasUsed ;
            console.log("\tgas per transaction from adrr1 to addr2:", gas);


        });

        it("Test scenario 2. Check whether all the transactions are successful", async function () {
            const { hardhatToken, owner, addr1, addr2 } = await loadFixture(
                deployTokenFixture
            );

            //from owner to addr1
            const ownerInitialBalance = await hardhatToken.balanceOf(owner.address);
            const addr1InitialBalance = await hardhatToken.balanceOf(addr1.address);

            await hardhatToken.transfer(addr1.address, 20);

            const ownerFinalBalance = await hardhatToken.balanceOf(owner.address);
            const addr1FinalBalance = await hardhatToken.balanceOf(addr1.address);

            expect(ownerFinalBalance).to.equal(ownerInitialBalance-BigInt(20));
            expect(addr1FinalBalance).to.equal(addr1InitialBalance+BigInt(20));
            //from addr1 to addr2
            const addr2InitialBalance = await hardhatToken.balanceOf(addr2.address);

            await hardhatToken.connect(addr1).transfer(addr2.address, 10);

            const addr2FinalBalance = await hardhatToken.balanceOf(addr2.address);
            const addr1FinalBalance2 = await hardhatToken.balanceOf(addr1.address);

            expect(addr1FinalBalance2).to.equal(addr1FinalBalance-BigInt(10));
            expect(addr2FinalBalance).to.equal(addr2InitialBalance+BigInt(10));

        });
    });
});