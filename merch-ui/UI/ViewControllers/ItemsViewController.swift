//
//  ItemsViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemsViewController: UIViewController, UITableViewDelegate {

    // MARK: - Outlets
    @IBOutlet weak var timerLabel: UILabel!
    @IBOutlet weak var nameLabel: UILabel!
    @IBOutlet weak var balanceLabel: UILabel!
    
    @IBOutlet weak var tableView: UITableView!
    
    let refreshControl = UIRefreshControl()
    
    // MARK: - Variables
    var autoLogoutTimer: Timer?
    let timeUntilAutoLogoutWarningAppears = 10
    let timeUntilAutoLogoutOccursAfterWarning = 500
    var timeSinceLastUserInteraction = 0 {
        didSet {
            switch timeSinceLastUserInteraction {
            case 0..<timeUntilAutoLogoutWarningAppears:
                timerLabel.text = "Welcome"
            case timeUntilAutoLogoutWarningAppears...timeUntilAutoLogoutWarningAppears + timeUntilAutoLogoutOccursAfterWarning:
                timerLabel.text = "Thank you, logging out in \(timeUntilAutoLogoutWarningAppears + timeUntilAutoLogoutOccursAfterWarning - timeSinceLastUserInteraction) sec."
            default:
                logout()
            }
        }
    }
    
    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.dataSource = ItemsDataSource.shared
        tableView.delegate = self
        
        (view as? UIUserInteractionCallBackView)?.callback = { [weak self] in
            self?.resetTimer()
        }
        
        refreshControl.tintColor = UIConstants.Colors.secondary
        refreshControl.addTarget(self, action: #selector(refresh(_:)), for: UIControlEvents.valueChanged)

        autoLogoutTimer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(timerDidTick(_:)), userInfo: nil, repeats: true)
        
        tableView.refreshControl = refreshControl
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        refresh(nil)
        
        nameLabel.text = UserModel.shared?.netID
        balanceLabel.text = "ℂ\(UserModel.shared?.balance ?? 0)"
        
        timeSinceLastUserInteraction = 0
        autoLogoutTimer?.fire()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        if UserModel.shared == nil {
            let alert = UIAlertController(title: "Internal Error", message: "Unable to load user", preferredStyle: .alert)
            let okAction = UIAlertAction(title: "Ok", style: .default, handler: { (_) in
                self.logout()
            })
            alert.addAction(okAction)
            present(alert, animated: true, completion: nil)
        }
    }

    override func viewWillDisappear(_ animated: Bool) {
        autoLogoutTimer?.invalidate()
        super.viewWillDisappear(animated)
    }
    
    // MARK: - Auto Logout
    func resetTimer() {
        timeSinceLastUserInteraction = 0
    }
    
    func timerDidTick(_ sender: Any) {
        timeSinceLastUserInteraction += 1
    }
    
    // MARK: - Actions
    @IBAction func logout() {
        dismiss(animated: true, completion: nil)
    }
    
    // MARK: - UITableViewDelegate
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 105
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: false)
    }
    
    // MARK: - Refresh Items
    func refresh(_ sender: Any?) {
        (sender as? UIRefreshControl)?.beginRefreshing()
        
        ItemsDataSource.shared.clear()
        tableView.reloadData()

        if let user = UserModel.shared {
            APIRequest.getItems(success: { (json) in
                DispatchQueue.main.async {
                    if let models = json as? [[String: Any]] {
                        ItemsDataSource.shared.populate(models: models)
                    }
                    self.tableView.reloadData()
                    (sender as? UIRefreshControl)?.endRefreshing()

                }
            }) { (error) in
                print(error)
                DispatchQueue.main.async {
                    self.tableView.reloadData()
                    (sender as? UIRefreshControl)?.endRefreshing()
                }
            }.perform(withAuthorization: user)
        }
    }
}
