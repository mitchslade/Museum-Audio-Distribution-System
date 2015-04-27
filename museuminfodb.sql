-- Database Creation Script
-- ESD Assignment
-- Written by: Paul Cadd
-- Date: 03-02-2015
-- Version: 1.0
-- 
-- 
-- 

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Create Database if not exists: `museuminfodb`
--
CREATE DATABASE IF NOT EXISTS museuminfodb;

-- --------------------------------------------------------

-- 
-- Set default database before creating tables etc
--
USE museuminfodb;

-- --------------------------------------------------------

--
-- Create the user to be able to access and work in the DB remotely
--
GRANT USAGE ON *.* to museum_db_admin@localhost IDENTIFIED BY '';
GRANT ALL PRIVILEGES ON museuminfodb.* TO museum_db_admin@localhost;
FLUSH PRIVILEGES;

-- --------------------------------------------------------
--
-- Table structure for table `admin`
--

CREATE TABLE IF NOT EXISTS `admin` (
  `id` int(11) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `last_log_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `difficulty`
--

CREATE TABLE IF NOT EXISTS `difficulty` (
  `difficulty_id` int(11) NOT NULL,
  `difficulty_code` int(1) DEFAULT NULL,
  `difficulty_description` varchar(30) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

--
-- Creatingdata for table `difficulty`
--

INSERT INTO `difficulty` (`difficulty_id`, `difficulty_code`, `difficulty_description`) VALUES
(1, 1, 'Brief Overview - 1 minute'),
(2, 2, 'Extended Overview - 2 minutes'),
(3, 3, 'In Depth Overview - 4 Minutes'),
(4, 4, 'Complete Boredom - 10 minutes');

-- --------------------------------------------------------

--
-- Table structure for table `exhibitions`
--

CREATE TABLE IF NOT EXISTS `exhibitions` (
  `exhibition_id` int(11) NOT NULL,
  `exhibition_description` varchar(255) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

--
-- Creating data for table `exhibitions`
--

INSERT INTO `exhibitions` (`exhibition_id`, `exhibition_description`, `price`) VALUES
(1, 'Egypt', '5.00'),
(2, 'China', '5.00');

-- --------------------------------------------------------

--
-- Table structure for table `languages`
--

CREATE TABLE IF NOT EXISTS `languages` (
  `language_id` int(11) NOT NULL,
  `country_code` varchar(2) DEFAULT NULL,
  `country_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=243 DEFAULT CHARSET=utf8;

--
-- Creating data for table `languages`
--

INSERT INTO `languages` (`language_id`, `country_code`, `country_name`) VALUES
(1, 'GB', 'United Kingdom'),
(2, 'CA', 'Canada'),
(3, 'AF', 'Afghanistan'),
(4, 'AL', 'Albania'),
(5, 'DZ', 'Algeria'),
(6, 'DS', 'American Samoa'),
(7, 'AD', 'Andorra'),
(8, 'AO', 'Angola'),
(9, 'AI', 'Anguilla'),
(10, 'AQ', 'Antarctica'),
(11, 'AG', 'Antigua and/or Barbuda'),
(12, 'AR', 'Argentina'),
(13, 'AM', 'Armenia'),
(14, 'AW', 'Aruba'),
(15, 'AU', 'Australia'),
(16, 'AT', 'Austria'),
(17, 'AZ', 'Azerbaijan'),
(18, 'BS', 'Bahamas'),
(19, 'BH', 'Bahrain'),
(20, 'BD', 'Bangladesh'),
(21, 'BB', 'Barbados'),
(22, 'BY', 'Belarus'),
(23, 'BE', 'Belgium'),
(24, 'BZ', 'Belize'),
(25, 'BJ', 'Benin'),
(26, 'BM', 'Bermuda'),
(27, 'BT', 'Bhutan'),
(28, 'BO', 'Bolivia'),
(29, 'BA', 'Bosnia and Herzegovina'),
(30, 'BW', 'Botswana'),
(31, 'BV', 'Bouvet Island'),
(32, 'BR', 'Brazil'),
(33, 'IO', 'British lndian Ocean Territory'),
(34, 'BN', 'Brunei Darussalam'),
(35, 'BG', 'Bulgaria'),
(36, 'BF', 'Burkina Faso'),
(37, 'BI', 'Burundi'),
(38, 'KH', 'Cambodia'),
(39, 'CM', 'Cameroon'),
(40, 'CV', 'Cape Verde'),
(41, 'KY', 'Cayman Islands'),
(42, 'CF', 'Central African Republic'),
(43, 'TD', 'Chad'),
(44, 'CL', 'Chile'),
(45, 'CN', 'China'),
(46, 'CX', 'Christmas Island'),
(47, 'CC', 'Cocos (Keeling) Islands'),
(48, 'CO', 'Colombia'),
(49, 'KM', 'Comoros'),
(50, 'CG', 'Congo'),
(51, 'CK', 'Cook Islands'),
(52, 'CR', 'Costa Rica'),
(53, 'HR', 'Croatia (Hrvatska)'),
(54, 'CU', 'Cuba'),
(55, 'CY', 'Cyprus'),
(56, 'CZ', 'Czech Republic'),
(57, 'DK', 'Denmark'),
(58, 'DJ', 'Djibouti'),
(59, 'DM', 'Dominica'),
(60, 'DO', 'Dominican Republic'),
(61, 'TP', 'East Timor'),
(62, 'EC', 'Ecuador'),
(63, 'EG', 'Egypt'),
(64, 'SV', 'El Salvador'),
(65, 'GQ', 'Equatorial Guinea'),
(66, 'ER', 'Eritrea'),
(67, 'EE', 'Estonia'),
(68, 'ET', 'Ethiopia'),
(69, 'FK', 'Falkland Islands (Malvinas)'),
(70, 'FO', 'Faroe Islands'),
(71, 'FJ', 'Fiji'),
(72, 'FI', 'Finland'),
(73, 'FR', 'France'),
(74, 'FX', 'France, Metropolitan'),
(75, 'GF', 'French Guiana'),
(76, 'PF', 'French Polynesia'),
(77, 'TF', 'French Southern Territories'),
(78, 'GA', 'Gabon'),
(79, 'GM', 'Gambia'),
(80, 'GE', 'Georgia'),
(81, 'DE', 'Germany'),
(82, 'GH', 'Ghana'),
(83, 'GI', 'Gibraltar'),
(84, 'GR', 'Greece'),
(85, 'GL', 'Greenland'),
(86, 'GD', 'Grenada'),
(87, 'GP', 'Guadeloupe'),
(88, 'GU', 'Guam'),
(89, 'GT', 'Guatemala'),
(90, 'GN', 'Guinea'),
(91, 'GW', 'Guinea-Bissau'),
(92, 'GY', 'Guyana'),
(93, 'HT', 'Haiti'),
(94, 'HM', 'Heard and Mc Donald Islands'),
(95, 'HN', 'Honduras'),
(96, 'HK', 'Hong Kong'),
(97, 'HU', 'Hungary'),
(98, 'IS', 'Iceland'),
(99, 'IN', 'India'),
(100, 'ID', 'Indonesia'),
(101, 'IR', 'Iran (Islamic Republic of)'),
(102, 'IQ', 'Iraq'),
(103, 'IE', 'Ireland'),
(104, 'IL', 'Israel'),
(105, 'IT', 'Italy'),
(106, 'CI', 'Ivory Coast'),
(107, 'JM', 'Jamaica'),
(108, 'JP', 'Japan'),
(109, 'JO', 'Jordan'),
(110, 'KZ', 'Kazakhstan'),
(111, 'KE', 'Kenya'),
(112, 'KI', 'Kiribati'),
(113, 'KP', 'Korea, Democratic People''s Republic of'),
(114, 'KR', 'Korea, Republic of'),
(115, 'XK', 'Kosovo'),
(116, 'KW', 'Kuwait'),
(117, 'KG', 'Kyrgyzstan'),
(118, 'LA', 'Lao People''s Democratic Republic'),
(119, 'LV', 'Latvia'),
(120, 'LB', 'Lebanon'),
(121, 'LS', 'Lesotho'),
(122, 'LR', 'Liberia'),
(123, 'LY', 'Libyan Arab Jamahiriya'),
(124, 'LI', 'Liechtenstein'),
(125, 'LT', 'Lithuania'),
(126, 'LU', 'Luxembourg'),
(127, 'MO', 'Macau'),
(128, 'MK', 'Macedonia'),
(129, 'MG', 'Madagascar'),
(130, 'MW', 'Malawi'),
(131, 'MY', 'Malaysia'),
(132, 'MV', 'Maldives'),
(133, 'ML', 'Mali'),
(134, 'MT', 'Malta'),
(135, 'MH', 'Marshall Islands'),
(136, 'MQ', 'Martinique'),
(137, 'MR', 'Mauritania'),
(138, 'MU', 'Mauritius'),
(139, 'TY', 'Mayotte'),
(140, 'MX', 'Mexico'),
(141, 'FM', 'Micronesia, Federated States of'),
(142, 'MD', 'Moldova, Republic of'),
(143, 'MC', 'Monaco'),
(144, 'MN', 'Mongolia'),
(145, 'ME', 'Montenegro'),
(146, 'MS', 'Montserrat'),
(147, 'MA', 'Morocco'),
(148, 'MZ', 'Mozambique'),
(149, 'MM', 'Myanmar'),
(150, 'NA', 'Namibia'),
(151, 'NR', 'Nauru'),
(152, 'NP', 'Nepal'),
(153, 'NL', 'Netherlands'),
(154, 'AN', 'Netherlands Antilles'),
(155, 'NC', 'New Caledonia'),
(156, 'NZ', 'New Zealand'),
(157, 'NI', 'Nicaragua'),
(158, 'NE', 'Niger'),
(159, 'NG', 'Nigeria'),
(160, 'NU', 'Niue'),
(161, 'NF', 'Norfork Island'),
(162, 'MP', 'Northern Mariana Islands'),
(163, 'NO', 'Norway'),
(164, 'OM', 'Oman'),
(165, 'PK', 'Pakistan'),
(166, 'PW', 'Palau'),
(167, 'PA', 'Panama'),
(168, 'PG', 'Papua New Guinea'),
(169, 'PY', 'Paraguay'),
(170, 'PE', 'Peru'),
(171, 'PH', 'Philippines'),
(172, 'PN', 'Pitcairn'),
(173, 'PL', 'Poland'),
(174, 'PT', 'Portugal'),
(175, 'PR', 'Puerto Rico'),
(176, 'QA', 'Qatar'),
(177, 'RE', 'Reunion'),
(178, 'RO', 'Romania'),
(179, 'RU', 'Russian Federation'),
(180, 'RW', 'Rwanda'),
(181, 'KN', 'Saint Kitts and Nevis'),
(182, 'LC', 'Saint Lucia'),
(183, 'VC', 'Saint Vincent and the Grenadines'),
(184, 'WS', 'Samoa'),
(185, 'SM', 'San Marino'),
(186, 'ST', 'Sao Tome and Principe'),
(187, 'SA', 'Saudi Arabia'),
(188, 'SN', 'Senegal'),
(189, 'RS', 'Serbia'),
(190, 'SC', 'Seychelles'),
(191, 'SL', 'Sierra Leone'),
(192, 'SG', 'Singapore'),
(193, 'SK', 'Slovakia'),
(194, 'SI', 'Slovenia'),
(195, 'SB', 'Solomon Islands'),
(196, 'SO', 'Somalia'),
(197, 'ZA', 'South Africa'),
(198, 'GS', 'South Georgia South Sandwich Islands'),
(199, 'ES', 'Spain'),
(200, 'LK', 'Sri Lanka'),
(201, 'SH', 'St. Helena'),
(202, 'PM', 'St. Pierre and Miquelon'),
(203, 'SD', 'Sudan'),
(204, 'SR', 'Suriname'),
(205, 'SJ', 'Svalbarn and Jan Mayen Islands'),
(206, 'SZ', 'Swaziland'),
(207, 'SE', 'Sweden'),
(208, 'CH', 'Switzerland'),
(209, 'SY', 'Syrian Arab Republic'),
(210, 'TW', 'Taiwan'),
(211, 'TJ', 'Tajikistan'),
(212, 'TZ', 'Tanzania, United Republic of'),
(213, 'TH', 'Thailand'),
(214, 'TG', 'Togo'),
(215, 'TK', 'Tokelau'),
(216, 'TO', 'Tonga'),
(217, 'TT', 'Trinidad and Tobago'),
(218, 'TN', 'Tunisia'),
(219, 'TR', 'Turkey'),
(220, 'TM', 'Turkmenistan'),
(221, 'TC', 'Turks and Caicos Islands'),
(222, 'TV', 'Tuvalu'),
(223, 'UG', 'Uganda'),
(224, 'UA', 'Ukraine'),
(225, 'AE', 'United Arab Emirates'),
(226, 'US', 'United States'),
(227, 'UM', 'United States minor outlying islands'),
(228, 'UY', 'Uruguay'),
(229, 'UZ', 'Uzbekistan'),
(230, 'VU', 'Vanuatu'),
(231, 'VA', 'Vatican City State'),
(232, 'VE', 'Venezuela'),
(233, 'VN', 'Vietnam'),
(234, 'VG', 'Virgin Islands (British)'),
(235, 'VI', 'Virgin Islands (U.S.)'),
(236, 'WF', 'Wallis and Futuna Islands'),
(237, 'EH', 'Western Sahara'),
(238, 'YE', 'Yemen'),
(239, 'YU', 'Yugoslavia'),
(240, 'ZR', 'Zaire'),
(241, 'ZM', 'Zambia'),
(242, 'ZW', 'Zimbabwe');

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE IF NOT EXISTS `orders` (
  `order_id` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `exhibition_id` int(11) DEFAULT NULL,
  `language_id` int(11) DEFAULT NULL,
  `difficulty_id` int(11) DEFAULT NULL,
  `order_date` date DEFAULT NULL,
  `paid_id` tinyint(1) DEFAULT NULL,
  `audio_path` varchar(255) DEFAULT NULL,
  `first_name` varchar(40) NOT NULL COMMENT 'Customers Last Name',
  `last_name` varchar(40) NOT NULL COMMENT 'Customers Last Name',
  `email_address` varchar(255) NOT NULL COMMENT 'Must be valid username@domain.com address',
  `payment_currency` varchar(40) NOT NULL COMMENT 'Currency paid in',
  `address_street` varchar(255) NOT NULL COMMENT 'Address First Line',
  `address_city` varchar(255) NOT NULL COMMENT 'Address Second Line',
  `address_county` varchar(255) NOT NULL COMMENT 'Address Third Line',
  `address_postcode` varchar(255) NOT NULL COMMENT 'Address Post Code'
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

--
-- Creating data for table `orders`
--

INSERT INTO `orders` (`order_id`, `user_id`, `exhibition_id`, `language_id`, `difficulty_id`, `order_date`, `paid_id`, `audio_path`, `first_name`, `last_name`, `payment_currency`, `address_street`, `address_city`, `address_county`, `address_postcode`) VALUES
(2, 4, 1, 1, 2, '2014-12-15', 1, '/home/gbcadp/museuminfo/audio/ex1_gb_2.ogg', '', '', '', '', '', '', ''),
(3, 2, 1, 1, 3, '2014-12-15', 1, '/home/gbcadp/museuminfo/audio/ex1_gb_3.ogg', '', '', '', '', '', '', ''),
(4, 3, 1, 1, 1, '2014-12-15', 1, '/home/gbcadp/museuminfo/audio/ex1_gb_1.ogg', '', '', '', '', '', '', '');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(11) NOT NULL,
  `userpin` int(4) DEFAULT NULL,
  `logon_status` tinyint(1) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

--
-- Creating data for table `users`
--

INSERT INTO `users` (`user_id`, `userpin`, `logon_status`) VALUES
(2, 2468, NULL),
(3, 1234, NULL),
(4, 6666, NULL);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`id`), ADD UNIQUE KEY `username` (`username`);

--
-- Indexes for table `difficulty`
--
ALTER TABLE `difficulty`
  ADD PRIMARY KEY (`difficulty_id`);

--
-- Indexes for table `exhibitions`
--
ALTER TABLE `exhibitions`
  ADD PRIMARY KEY (`exhibition_id`);

--
-- Indexes for table `languages`
--
ALTER TABLE `languages`
  ADD PRIMARY KEY (`language_id`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`order_id`), ADD KEY `user_id` (`user_id`), ADD KEY `exhibition_id` (`exhibition_id`), ADD KEY `language_id` (`language_id`), ADD KEY `difficulty_id` (`difficulty_id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`user_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `admin`
--
ALTER TABLE `admin`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `difficulty`
--
ALTER TABLE `difficulty`
  MODIFY `difficulty_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `exhibitions`
--
ALTER TABLE `exhibitions`
  MODIFY `exhibition_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `languages`
--
ALTER TABLE `languages`
  MODIFY `language_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=243;
--
-- AUTO_INCREMENT for table `orders`
--
ALTER TABLE `orders`
  MODIFY `order_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=5;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`),
ADD CONSTRAINT `orders_ibfk_2` FOREIGN KEY (`exhibition_id`) REFERENCES `exhibitions` (`exhibition_id`),
ADD CONSTRAINT `orders_ibfk_3` FOREIGN KEY (`language_id`) REFERENCES `languages` (`language_id`),
ADD CONSTRAINT `orders_ibfk_4` FOREIGN KEY (`difficulty_id`) REFERENCES `difficulty` (`difficulty_id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
