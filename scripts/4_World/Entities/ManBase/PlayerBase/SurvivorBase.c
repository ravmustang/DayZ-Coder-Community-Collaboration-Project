class SurvivorBase extends PlayerBaseClient {
    private string PlayerIdentityName = "empty";

    string GetPlayerIdentityName() {
        return this.PlayerIdentityName;
    }

    void SetPlayerIdentityName(string name) {
        this.PlayerIdentityName = name;
    }
}