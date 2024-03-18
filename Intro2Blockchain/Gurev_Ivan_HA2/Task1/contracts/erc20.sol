// contracts/MyToken.sol
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";

contract MyToken is ERC20 {
    address payable public owner;
    constructor() ERC20("MyToken", "MTK") {
        owner = payable(msg.sender);
        _mint(msg.sender, 1000000 * 10 ** 18);
    }
}